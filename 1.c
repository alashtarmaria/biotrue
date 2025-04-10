#define DISPLAY_ONOFF LATG.F7

char text_to_send[] = "t0.txt=\"hello\"";
char rxx;

void Display_TurnOn() {
    TRISG.F7 = 0;
    DISPLAY_ONOFF = 1;
}

/*
int j;
char hex_high;
char  hex_low;


void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;
    ANSELB = 0x00;

    // PPS sabit (elleme!)
    PPS_Mapping(65, _INPUT,  _U1RX);  // Nextion TX ? MCU RX
    PPS_Mapping(64, _OUTPUT, _U1TX);  // MCU TX ? Nextion RX
    PPS_Mapping(69, _INPUT,  _U3RX);  // Terminal RX
    PPS_Mapping(68, _OUTPUT, _U3TX);  // Terminal TX

    Display_TurnOn();

    UART1_Init(9600);  // Nextion
    UART3_Init(9600);  // Terminal

    Delay_ms(3000);  // Ekran açilsin

    // Sayfa 0'a geç
    UART3_Write_Text("[DEBUG] Giden: page 0\r\n");
    UART1_Write_Text("page 0");
    UART1_Write(0xFF); UART1_Write(0xFF); UART1_Write(0xFF);
    Delay_ms(200);

    // Yaziyi gönder, byte byte
    UART3_Write_Text("[DEBUG] Giden: ");
    for (j = 0; text_to_send[j] != '\0'; j++) {
        UART1_Write(text_to_send[j]);
        UART3_Write(text_to_send[j]); // terminalde de görün
    }

    // 3x 0xFF
    UART1_Write(0xFF); UART1_Write(0xFF); UART1_Write(0xFF);
    UART3_Write_Text(" [0xFF][0xFF][0xFF]\r\n");

    // Gelen veriyi dinle ve HEX göster
    while (1) {
        if (UART1_Data_Ready()) {
            rxx = UART1_Read();
            UART3_Write_Text("[NEXTION CEVAP]: ");
            UART3_Write(rxx);
            UART3_Write_Text(" (HEX: ");

             hex_high = ((rxx >> 4) & 0x0F);
             hex_low  = (rxx & 0x0F);

            if (hex_high < 10) hex_high += '0';
            else hex_high += ('A' - 10);

            if (hex_low < 10) hex_low += '0';
            else hex_low += ('A' - 10);

            UART3_Write(hex_high);
            UART3_Write(hex_low);
            UART3_Write_Text(")\r\n");
        }
    }
}
 */
 
 
