    echo accuityLocation-ReqBankByNameANDTown/Country/BankName/Town $1:$2
    ./GET.sh 'accuityLocation-ReqBankByNameANDTown/Country/BankName/Town?Country="VENEZUELA"&BankName="BANCO"&Town="CARACAS"' $1 $2
    