echo PUT $1 $2:$3
cat ./$1.put
curl -X PUT     "http://$2:$3/$1" -H "Accept: application/json" -H "Content-Type: application/json" -d @./$1.put
