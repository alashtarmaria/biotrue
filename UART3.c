
void main() {
    char received_data;

    PLLFBD = 70;
    CLKDIV = 0x0000;

    // PPS Ayarlari
    PPS_Mapping(69, _INPUT,  _U3RX);  // UART3 RX
    PPS_Mapping(68, _OUTPUT, _U3TX);  // UART3 TX

    // UART3 Baslat
    UART3_Init(9600);
    Delay_ms(100);

    // LED Pin Ayari (RB6 çikis olarak ayarlandi)
    TRISB.B6 = 0;
    LATB.B6 = 0; // Baslangiçta LED kapali

    UART3_Write_Text("UART3 Started...\r\n");
    UART3_Write(10);
    UART3_Write(13);
    UART3_Write_Text("A = Light led | B = Eextinguish led!");
    UART3_Write(10);
    UART3_Write(13);

    while (1) {
        // **RX'den gelen veriyi oku**
        if (UART3_Data_Ready()) {  // Eger UART veri aldiysa
            received_data = UART3_Read();  // Gelen veriyi oku

            // **LED Kontrolü**
            if (received_data == 'A') {
                LATB.B6 = 1;  // LED YAN
                UART3_Write_Text("Led ON!\r\n");
            }
            else if (received_data == 'B') {
                LATB.B6 = 0;  // LED SÖN
                UART3_Write_Text("Led OFF!\r\n");
            }
            else {
                UART3_Write_Text("Invalid character!\r\n");
            }
        }
    }
}


















