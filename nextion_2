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

void nextion_go_to_page0() {
    UART1_Write_Text("page 0");
    UART1_Write(0xFF);
    UART1_Write(0xFF);
    UART1_Write(0xFF);
    Delay_ms(100);  // Sayfa degisince objeler aktif olsun
}

void main() {
    // PLL & Clock ayari
    PLLFBD = 70;
    CLKDIV = 0x0000;

    // Tüm pinleri dijital yap
    ANSELB = 0x00;

    // PORTB çikis olarak ayarlanir
    TRISB = 0x00;
    close_all_leds();

    // UART PPS ayari
    PPS_Mapping(100, _INPUT,  _U1RX);  // RF5
    PPS_Mapping(101, _OUTPUT, _U1TX);  // RF4

    UART1_Init(9600);
    Delay_ms(3000); // Nextion ekranin açilmasini bekle

    // Sayfa 0'a geç (önlem amaçli)
    nextion_go_to_page0();

    // t0 objesine yazi gönder
    nextion_send_text("Test basladi");

    while(1); // Sonsuz döngü
}
