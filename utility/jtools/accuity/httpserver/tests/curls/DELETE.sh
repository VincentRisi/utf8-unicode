echo DELETE $1 $2 $3
curl -X DELETE "http://$1:$2/$3" -H "Accept: application/json"
