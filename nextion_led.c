char uart_rd;

void close_all_leds() {
    LATB = 0x00;
}

void nextion_send_text(char* text) {
    UART1_Write_Text("t0.txt=\"");
    UART1_Write_Text(text);
    UART1_Write_Text("\"");
    UART1_Write(0xFF);
    UART1_Write(0xFF);
    UART1_Write(0xFF);
}

void main() {
    // Sistem Ayarlari
    PLLFBD = 70;
    CLKDIV = 0x0000;

    ANSELB = 0x00;
    TRISB = 0x00;
    close_all_leds();

    // UART PPS
    PPS_Mapping(100, _INPUT,  _U1RX);  // RF5
    PPS_Mapping(101, _OUTPUT, _U1TX);  // RF4

    UART1_Init(9600);
    Delay_ms(100);

    nextion_send_text("Hello"); // Baslangiç mesaji
    Delay_ms(2000);
    while (1) {
        LATB = 0xFF; // LED'leri yak
        nextion_send_text("Led yandi");
        Delay_ms(1500);

        LATB = 0x00; // LED'leri kapat
        nextion_send_text("Led sondu");
        Delay_ms(1500);
    }
}
