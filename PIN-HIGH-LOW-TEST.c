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

    // RD0 ve RD1 pinlerini output olarak ayarla
    TRISD.F0 = 0;
    TRISD.F1 = 0;

    // Baslangiçta her iki pin LOW olsun
    LATD.F0 = 0;
    LATD.F1 = 0;

    UART3_Write_Text("UART3 Started...\r\n");
    UART3_Write(10);
    UART3_Write(13);
    UART3_Write_Text("A = RD0-RD1 HIGH | B = RD0-RD1 LOW");
    UART3_Write(10);
    UART3_Write(13);
    

    while (1) {
        if (UART3_Data_Ready()) {
            received_data = UART3_Read();

            if (received_data == 'A') {
                LATD.F0 = 1;
                LATD.F1 = 1;
                UART3_Write_Text("\nRD0 ve RD1 HIGH yapildi\r\n");
            }
            else if (received_data == 'B') {
                LATD.F0 = 0;
                LATD.F1 = 0;
                UART3_Write_Text("RD0 ve RD1 LOW yapildi\r\n");
            }
            else {
                UART3_Write_Text("Gecersiz karakter!\r\n");
            }
        }
    }
}
