echo "---   compile start    ---"
gcc gen.c -Wall -o gen
gcc genmain.c scheduler.c threads.c -Wall -o shin

while [ 1 ]
do
    echo "--- start running(gen) ---"
    ./gen $1
    echo "---  start running(1)  ---"
    ./cmd.sh shin > ur_out
    echo "---  start running(2)  ---"
    ./cmd.sh kiwi_to_replace_remi > out
    echo "---  start comparing   ---"
    DIFF=$(diff ur_out out)

    if [ "$DIFF" != "" ]
    then
        echo "Wrong Output"
        echo "$(DIFF)"
        break
    fi
done
