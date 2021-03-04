import os

for energy in range(10, 90, 10):
    rootfile = '~/workplace/CaloSim/script/output/gamma_' + str(energy) + 'GeV/caloHits.root'
    os.system('ln -sf ' + rootfile + ' caloHits.root')

    recofile = 'reco_gamma_' + str(energy) + 'GeV.root'
    os.system('./bin/runRec -i settings/test.xml -n 3000 > /dev/null')

    os.system('mv -f test.root ' + recofile)
    print(str(energy) + ' GeV done.')
