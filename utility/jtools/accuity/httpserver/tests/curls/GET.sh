echo GET $1 $2:$3
curl -X GET "http://$2:$3/$1" -H "Accept: application/json"
