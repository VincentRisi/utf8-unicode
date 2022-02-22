    echo accuityLocation-ReqBankByName/Country/BankName $1:$2
    ./GET.sh 'accuityLocation-ReqBankByName/Country/BankName?Country="VENEZUELA"&BankName="BANCO"' $1 $2
    