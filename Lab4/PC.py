import serial as ser

pingPongNum = 10

#in this example we sends a character and ping pong the input character "pingPongNum" times before the port closes
def print_menu():
    print("Menu")
    print("1. Blink RGB LED, color by color with delay of X[ms]")
    print("2. Count up onto LCD screen with delay of X[ms]")
    print("3. Circular tone series via Buzzer with delay of X[ms]")
    print("4. Get delay time X[ms]:")
    print("5. LDR 3-digit value [v] onto LCD")
    print("6. Clear LCD screen")
    print("7. state 7")
    print("8. Show menu")
    print("9. Sleep")


def switch(num):
    if num == '4':
        inChar_local = input("Enter X:")
        return inChar_local
    elif num == '8':
        print_menu()
        return '0'
    elif num == '7':
        return '#'
    else:
        return '0'

def main():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec where the read and write operations are blocking,
                                # after the timeout the program continues
    enableTX = True
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    """
    count = 1
    char = ''
    inChar = input("Enter char:")
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    """
    print_menu()
    str = []
    i = 0
    while(1):

        while (s.in_waiting > 0):  # while the input buffer isn't empty
            enableTX = False
            char = s.read(size = 1)  # read 1 char from the input buffer
            str.append(char.decode("ascii"))
            i+=1

            if (s.in_waiting == 0 and i ==15):
                print(''.join(str))
                str = []
                i = 0
                enableTX = True  # enable transmission to echo the received data

        while (s.out_waiting > 0 or enableTX):
            inChar = input("Enter menu option:")
            bytesChar = bytes(inChar, 'ascii')
            s.write(bytesChar)
            ans = switch(inChar)
            s.reset_input_buffer()
            if ans != '0' and inChar!='7':
                for c in ans:
                    bytesChar_data = bytes(c, 'ascii')
                    s.write(bytesChar_data)
                bytesChar_data = bytes('\n', 'ascii')
                s.write(bytesChar_data)
            elif inChar=='7':
                enableTX = False

      #  if (s.out_waiting == 0):
       #     enableTX = False
        """
            s.write(char)
            count = count + 1
            if (s.out_waiting == 0):
                enableTX = False
        if count == pingPongNum + 1:
            s.close()
            break
            """

if __name__ == '__main__':
    main()