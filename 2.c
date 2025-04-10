void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    ANSELD = 0x00;  // RD portu dijital

    // UART3 PPS (opsiyonel terminal log için)
    PPS_Mapping(69, _INPUT,  _U3RX);  // Terminal RX
    PPS_Mapping(68, _OUTPUT, _U3TX);  // Terminal TX
    UART3_Init(9600);
    Delay_ms(100);
    UART3_Write_Text("RP64–RP67 toggle basladi (PORTD üzerinden)\r\n");

    // RD0–RD3 çıkış ayarı
    TRISD.F0 = 0;  // RP64
    TRISD.F1 = 0;  // RP65
    TRISD.F2 = 0;  // RP66
    TRISD.F3 = 0;  // RP67

    // Başlangıç: LOW
    LATD.F0 = 0;
    LATD.F1 = 0;
    LATD.F2 = 0;
    LATD.F3 = 0;

    while (1) {
        // HIGH
        LATD.F0 = 1;
        LATD.F1 = 1;
        LATD.F2 = 1;
        LATD.F3 = 1;
        UART3_Write_Text("PINLER HIGH (RP64–RP67)\r\n");
        Delay_ms(500);

        // LOW
        LATD.F0 = 0;
        LATD.F1 = 0;
        LATD.F2 = 0;
        LATD.F3 = 0;
        UART3_Write_Text("PINLER LOW (RP64–RP67)\r\n");
        Delay_ms(500);
    }
}
