#define DISPLAY_ONOFF LATG.F7

char r;
char h1, h2;

void Display_TurnOn() {
    TRISG.F7 = 0;
    DISPLAY_ONOFF = 1;
}

void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;
    ANSELB = 0x00;

    // Nextion ekrani aç
    Display_TurnOn();

    // PPS ayarlari (degistirme!)
    PPS_Mapping(65, _INPUT,  _U1RX);  // UART1 RX (Nextion TX)
    PPS_Mapping(64, _OUTPUT, _U1TX);  // UART1 TX (Nextion RX)
    PPS_Mapping(69, _INPUT,  _U3RX);  // UART3 RX (Terminal)
    PPS_Mapping(68, _OUTPUT, _U3TX);  // UART3 TX (Terminal)

    UART1_Init(9600);  // Nextion ile haberlesme
    UART3_Init(9600);  // Terminal için
    Delay_ms(3000);    // Ekran tam açilsin

    UART3_Write_Text("[DEBUG] get t0.txt gönderiliyor...\r\n");

    // Komutu gönder: get t0.txt + 0xFF x3
    UART1_Write_Text("get t0.txt");
    UART1_Write(0xFF);
    UART1_Write(0xFF);
    UART1_Write(0xFF);

    UART3_Write_Text("[DEBUG] Cevap:\r\n");

    // Gelen veriyi karakter + HEX olarak göster
    while (1) {
        if (UART1_Data_Ready()) {
            r = UART1_Read();

            UART3_Write(r); // karakter olarak bas
            UART3_Write_Text(" (HEX: ");

            h1 = ((r >> 4) & 0x0F) + '0';
            h2 = (r & 0x0F) + '0';
            if (h1 > '9') h1 += 7;
            if (h2 > '9') h2 += 7;

            UART3_Write(h1);
            UART3_Write(h2);
            UART3_Write_Text(")\r\n");
        }
    }
}
