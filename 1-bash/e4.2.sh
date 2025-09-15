#!/bin/bash
echo "Enter the first number:"
read num1
echo "Enter the second number:"
read num2
echo "Enter the operation (+, -, *, /):"
read operation

case $operation in
  +)
    echo "$num1 + $num2 = $(($num1 + $num2))"
    ;;
  -)
    echo "$num1 - $num2 = $(($num1 - $num2))"
    ;;
  \*)
    echo "$num1 * $num2 = $(($num1 * $num2))"
    ;;

/)
      if [[ $num2 -ne 0 ]]; then
        echo "$num1 / $num2 = $(($num1 / $num2))"
      else
        echo "Division by zero is not allowed"
      fi
      ;;
    *)
      echo "Invalid operation"
      ;;
esac
