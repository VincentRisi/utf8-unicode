echo PATCH $1 $2:$3
cat ./$1.patch
curl -X PATCH   "http://$2:$3/$1" -H "Accept: application/json" -H "Content-Type: application/json" -d @./$1.patch

