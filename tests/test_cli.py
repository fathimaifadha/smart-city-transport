"""Run after building: python tests/test_cli.py path/to/program.exe"""
import math
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXE = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else ROOT / 'SmartCityTransport.exe'
checks = 0

def run(lines):
    result = subprocess.run([str(EXE)], input='\n'.join(map(str, lines)) + '\n',
                            text=True, capture_output=True, timeout=5, check=True)
    return result.stdout

def expect(lines, *messages):
    global checks
    output = run(lines)
    for message in messages:
        assert message in output, (lines, message, output)
    checks += 1
    return output

expect([8, 10], 'Plan a journey with option 6 first', 'Total stored passenger demand: 840')
expect([6, 1, 1, 8, 10], '07:00 - 07:05', '07:05 - 07:09', '07:09 - 07:18',
       'Arrival: 07:18', 'Bus only: 27.00 minutes', 'Train only: 41.00 minutes',
       'Time saved versus Bus only: 9.00 minutes', 'Time saved versus Train only: 23.00 minutes',
       'Utilisation: 16.3%', 'Utilisation: 72.2%', 'Unused capacity: 50')
expect([6, 1, 2, 10], 'Passengers: 70', 'Arrival: 13:18', 'Buses required: 2')
expect([6, 1, 3, 10], 'Passengers: 240', 'Arrival: 17:18', 'Buses required: 4')
expect([6, 2, 'Colombo Fort', 'Nugegoda', '23:55', 130, 8, 10],
       'Arrival: 00:13 (+1 day)', 'Total stored passenger demand: 840')
expect([6, 2, 'Colombo Fort', 'Kottawa', '07:00', 130, 8, 10],
       'Arrival: 07:33', 'Bus only: No route available')
expect([6, 2, 'Borella', 'Borella', '07:00', 130, 8, 10],
       'Arrival: 07:00', 'Vehicles required: 0', 'capacity utilisation is not applicable')
for time in ['24:00', '07:60', '7:00', 'noon', '', '07:00x', '-1:00']:
    expect([6, 2, 'Borella', 'Nugegoda', time, 130, 10], 'Invalid departure time')
for count in ['0', '-1', '1.5', 'abc', '2147483648']:
    expect([6, 2, 'Borella', 'Nugegoda', '07:00', count, 10], 'positive whole number')
expect([6, 2, 'Borella', 'Nugegoda', '07:00', 2147483647, 8, 10],
       'Available capacity: 2147483700', 'Unused capacity: 53')
for record in [0, 7, -1, 'abc', '1x']:
    expect([6, 1, record, 10], 'record number')
expect([6, 2, 'Missing', 'Nugegoda', '07:00', 130, 8, 10],
       'Location not found.', 'Plan a journey with option 6 first')
expect([6, 1, 1, 6, 1, 99, 8, 5, 'Colombo Fort', 'Nugegoda', 10],
       'LAST SUCCESSFULLY PLANNED JOURNEY', 'Total travel time: 18 minutes')
expect([7, 'Colombo Fort', 10], 'Colombo Fort, Maradana, Borella, Wellawatte')
for option in [1, 2, 3, 4, 9]:
    expect([option, 10], 'Program terminated.')

# Independently compute filtered shortest times with Floyd-Warshall,
# then compare every pair with options 5, 6 and 8.
source = (ROOT / 'src/main.cpp').read_text()
names = re.findall(r'city.addLocation\("([^"]+)"\)', source)
routes = re.findall(r'city.addRoute\("([^"]+)",\s*"([^"]+)",\s*[\d.]+,\s*(\d+),\s*"(Bus|Train)"', source)
tables = {}
for mode in ['', 'Bus', 'Train']:
    dist = [[0 if i == j else math.inf for j in names] for i in names]
    for a, b, t, kind in routes:
        if mode and kind != mode:
            continue
        i, j = names.index(a), names.index(b)
        dist[i][j] = dist[j][i] = min(dist[i][j], float(t))
    for k in range(len(names)):
        for i in range(len(names)):
            for j in range(len(names)):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
    tables[mode] = dist
for i, a in enumerate(names):
    for j, b in enumerate(names):
        output = expect([5, a, b, 6, 2, a, b, '07:00', 130, 8, 10],
                        f'Total travel time: {tables[""][i][j]:g} minutes')
        for mode in ['Bus', 'Train']:
            t = tables[mode][i][j]
            expected = f'{mode} only: {t:.2f} minutes' if math.isfinite(t) else f'{mode} only: No route available'
            assert expected in output, (a, b, expected)
print(f'Passed {checks} scenarios, including all 64 location pairs and both mode filters.')
