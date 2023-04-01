#!/usr/bin/python3
import subprocess
import os

projects = []
blacklist = ['cmake','everything','usb','hello']
failures = []
def grab_demo_folders():
    for item in os.scandir('../demoProject'):
        if item.is_dir():
            if not (item.name in blacklist):
                #print(item.name)
                projects.append(item.name)
    projects.sort()

def exec_makethingie(title, cmds):
    res=0
    print("\t"+title+"...")
    try:
        res=0 # Assume it taises an excception if a problem occurs
        subprocess.check_call(cmds,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL) #, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except Exception as e:
        print("\t OS ERROR")
        res=-1
    if res!=0:
        print("\t"+title+"FAILED")
        return False
    print("\t\tok")
    return True
##
def build_single(arch, mcu, proj,extra,extra_title):
    cwd=os.getcwd()
    os.chdir(cwd+'/../demoProject/'+str(proj))
    #print(os.getcwd())
    #-- Cleanup
    build_dir='build'+'_'+str(arch)+'_'+str(mcu)+str(extra_title)
    subprocess.check_call(['rm','-Rf',build_dir])
    subprocess.check_call(['rm','-f','lnArduino'])
    subprocess.check_call(['ln','-s','../..','lnArduino'])
    subprocess.check_call(['mkdir',build_dir])
    os.chdir(cwd+'/../demoProject/'+str(proj)+'/'+build_dir)
    #
    cmakeParam= [ 'cmake', '-G', 'Ninja','-DLN_ARCH='+arch,'-DLN_MCU='+mcu,extra,'..']
    makeParam = ['ninja','-v']
    if exec_makethingie("CMAKE",cmakeParam)==False:
        return False
    if exec_makethingie("BUILD",makeParam)==False:
        return False
    return True
    #os.chdir(cwd)
    #print(str(projects))
def build_all(arch, mcu,extra, extra_title):
    cwd=os.getcwd()
    for i in projects:
        print("[",arch,"/",mcu,"/",extra_title,"]:",i)
        os.chdir(cwd)
        if False==build_single(arch,mcu,i,extra,extra_title):
            failures.append( str(arch)+'_'+str(mcu)+'_'+str(i)+str(extra_title) )
    os.chdir(cwd)
    pass

print("-- Build all --")
grab_demo_folders()
with_clang = "-DUSE_CLANG=True"
build_all("ARM","M3","","gcc")
build_all("ARM","M3",with_clang,"clang")
build_all("ARM","M4","","gcc")
build_all("ARM","M3",with_clang,"clang")
build_all("RISCV","CH32V3x","","gcc")
build_all("RISCV","CH32V3x",with_clang,"clang")
# Obsolete build_all("RISCV","VF103")
print("-- Failures --")
for i in failures:
    print("\t"+i)
print("-- Done --")

