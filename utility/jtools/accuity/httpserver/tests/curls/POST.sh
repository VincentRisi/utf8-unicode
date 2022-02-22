echo POST $1 $2:$3
cat ./$1.post
curl -X POST    "http://$2:$3/$1" -H "Accept: application/json" -H "Content-Type: application/json" -d @./$1.post
