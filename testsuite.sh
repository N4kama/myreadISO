#!/bin/sh

if [ "$#" -ne 3 ]; then
    echo "give <your exe> <correction exe> <iso file> as parameters please."
    exit
fi

mkdir mkdir -p example example/MP3 example/PICS example/PICS example/PICS/SECRET
touch example/MP3/TEST1.MP3 example/MP3/TEST2.MP3 example/PICS/ORDURE.JPG
touch example/PICS/SECRET/ORDURE.MP3

echo > example/README.TXT
echo "===============================================" >> example/README.TXT
echo "Ce fichier ne contient aucune information utile" >> example/README.TXT
echo "===============================================" >> example/README.TXT
echo >> example/README.TXT
echo "Les ACUs" >> example/README.TXT
echo >> example/README.TXT

mkisofs -o example.iso example/

rm -rf example/

list=$(echo 'help' 'info' 'ls')

for elm in $list; do
    $(echo "$elm" | "$1" "$3" > my_res)
    $(echo "$elm" | "$2" "$3" > res)
    the_diff=$(diff my_res res)
    if [ "$?" -eq 1 ]; then
        printf "RUNNING COMMAND : \t$(elm)\n\nRESULT : "
        printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
        printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
    else
        printf "RUNNING COMMAND : \t$elm\n\nRESULT :  \e[92mOK\e[0m"
        printf "\n\nSTUDENT'S RESULT :\n\n"
        printf "$(cat my_res)\n\n"
    fi
    echo -----------------------------------------------------------------------
    echo
done

elm="cd PICS\nls\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \tcd PICS\$ls\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \tcd PICS\$ls\n\nRESULT :  \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "$(cat my_res)\n\n"
fi
echo --------------------------------------------------------------------------
echo

elm="cd ..\nls\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \tcd ..\$ls\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \tcd ..\$ls\n\nRESULT :  \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "$(cat my_res)\n\n"
fi
echo --------------------------------------------------------------------------
echo

elm="tree\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \ttree ..\$ls\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \ttree ..\$ls\n\nRESULT :  \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "$(cat my_res)\n\n"
fi
echo --------------------------------------------------------------------------
echo

elm="tree MP3\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \ttree MP3 ..\$ls\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \ttree MP3..\$ls\n\nRESULT :  \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "$(cat my_res)\n\n"
fi
echo --------------------------------------------------------------------------
echo

elm="cat README.TXT\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \tcat README.TXT\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \tcat README.TXT\n\nRESULT :  \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "$(cat my_res)\n\n"
fi
echo --------------------------------------------------------------------------
echo

elm="get README.TXT\n"
$(printf "$elm" | "$1" "$3" > my_res)
$(printf "$elm" | "$2" "$3" > res)
the_diff=$(diff my_res res)
if [ "$?" -eq 1 ]; then
    printf "RUNNING COMMAND : \tget README.TXT\n\nRESULT : "
    printf "\e[91mBAD\e[0m\n\nSTUDENT'S RESULT :\n"
    printf "\n$(cat my_res)\n\nSHOULD BE :\n\n$(cat res)\n\n"
else
    printf "RUNNING COMMAND : \tget README.TXT\n\nRESULT : \e[92mOK\e[0m"
    printf "\n\nSTUDENT'S RESULT :\n\n"
    printf "/!\ you should cat it yourself to prove it worked.. /!\ \n\n"
fi
echo --------------------------------------------------------------------------
echo

rm my_res res