import os

particle = 'gamma'
maxEnergy = 90

for energy in range(10, maxEnergy, 10):
    rootfile = '~/workplace/CaloSim/script/output/' + particle + '_' + str(energy) + 'GeV/caloHits.root'
    os.system('ln -sf ' + rootfile + ' caloHits.root')

    recofile = 'reco_' + particle + '_' + str(energy) + 'GeV.root'
    os.system('./bin/runRec -i settings/test.xml -n 10000 > /dev/null')

    os.system('mv -f test.root ' + recofile)
    print(str(energy) + ' GeV done.')
