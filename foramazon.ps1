rm -recurse .\PackagedServer\ForAmazon -force
Copy-Item .\PackagedServer\WindowsServer\ -Destination .\PackagedServer\ForAmazon -Exclude *.pdb,*.log -Recurse
Copy-Item "D:\dev\UnrealEngine\Engine\Extras\Redist\en-us\UE4PrereqSetup_x64.exe" -Destination .\PackagedServer\ForAmazon
Copy-Item amazon_install.bat -Destination .\PackagedServer\ForAmazon\install.bat