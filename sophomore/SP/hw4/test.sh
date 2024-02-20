# run sh test.sh 2&>a.out

echo "---  start running(N=1)  ---"
time ./bench 1 
echo "---  start running(N=2)  ---"
time ./bench 2 
echo "---  start running(N=5)  ---"
time ./bench 5 
echo "---  start running(N=10)  ---"
time ./bench 10 
echo "---  start running(N=30)  ---"
time ./bench 30 
echo "---  start running(N=50)  ---"
time ./bench 50 
echo "---  start running(N=75)  ---"
time ./bench 75 
echo "---  start running(N=100)  ---"
time ./bench 100 
echo "---  start running(N=125)  ---"
time ./bench 125 
echo "---  start running(N=150)  ---"
time ./bench 150 
