sudo rm -f /dev/calculator_first
sudo rm -f /dev/calculator_second
sudo rm -f /dev/calculator_operation
sudo rm -f /dev/calculator_result
sudo rmmod calculator

make 
echo ""
sudo insmod ./calculator.ko

sudo mknod /dev/calculator_first c 300 0
sudo mknod /dev/calculator_second c 300 1
sudo mknod /dev/calculator_operation c 300 2
sudo mknod /dev/calculator_result c 300 3

sudo chmod a+r+w /dev/calculator_first
sudo chmod a+r+w /dev/calculator_second
sudo chmod a+r+w /dev/calculator_operation
sudo chmod a+r+w /dev/calculator_result

echo ""
echo "Test 1:"
echo "17 + 56 = "
echo "17" > /dev/calculator_first
echo "56" > /dev/calculator_second
echo "+" > /dev/calculator_operation
cat /dev/calculator_result

echo ""
echo "Test 2"
echo "123 - 82 = "
echo "123" > /dev/calculator_first
echo "82" > /dev/calculator_second
echo "-" > /dev/calculator_operation
cat /dev/calculator_result

echo ""
echo "Test 3"
echo "19 * 4 = "
echo "19" > /dev/calculator_first
echo "4" > /dev/calculator_second
echo "*" > /dev/calculator_operation
cat /dev/calculator_result

echo ""
echo "Test 4"
echo "152 / 8 = "
echo "152" > /dev/calculator_first
echo "8" > /dev/calculator_second
echo '/' > /dev/calculator_operation
cat /dev/calculator_result

echo ""
echo "Test 5"
echo "18 / 0 = "
echo "18" > /dev/calculator_first
echo "0" > /dev/calculator_second
echo "/" > /dev/calculator_operation
cat /dev/calculator_result

echo ""
echo "Test 6"
echo "7 g 8 = "
echo "7" > /dev/calculator_first
echo "8" > /dev/calculator_second
echo "y" > /dev/calculator_operation
cat /dev/calculator_result

dmesg > info.txt
