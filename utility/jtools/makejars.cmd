@setlocal

@PATH="C:\java\jdk1.6.0_43\bin"
@set build=C:\vlab\utility\jtools\build

@pushd .\jportal\out\production\jportal
    jar cfe %build%\jportal.jar bbd/jportal/Compiler bbd/jportal/*.class bbd/jportal/decompiler/*.class bbd/jportal/util/*.class
@popd

@pushd .\crackle\out\production\crackle
    jar cfe %build%\crackle.jar bbd/crackle/Compiler bbd/crackle/*.class bbd/crackle/rdc/*.class bbd/crackle/rpc/*.class bbd/crackle/rw/*.class bbd/crackle/util/*.class
@popd

@pause
