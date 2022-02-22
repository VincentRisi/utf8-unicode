gets = '''\
accuityCorrespondent-Get/FinId/CorrCurrency
accuityCorrespondent-SwiftList/FinId/CorrCurrency
'''.splitlines()

for get in gets:
  parts = get.split('/')
  part1 = parts[0]
  with open(f'{part1}-get.sh', 'wt') as ofile:
    part2 = ''
    part3 = ''
    p3del = '?'
    for bit in parts[1:]:
      part2 += f'/{bit}'
      part3 += f'{p3del}{bit}=0'
      p3del = '&'
    ofile.write(f'''\
    echo {part1}{part2} $1:$2
    ./GET.sh "{part1}{part2}{part3}" $1 $2
    ''')

