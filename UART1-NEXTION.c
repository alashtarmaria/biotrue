#define DISPLAY_ONOFF LATG.F7

void InitUART1(unsigned long baudrate);
void InitUART3(unsigned long baudrate);
void UART1_W(char *txt);
void UART3_W(char *txt);
void UART3_Write_Hex(unsigned char num);
void Display_TurnOn();

void InitUART1(unsigned long baudrate) {
    TRISD.F0 = 0;
    TRISD.F1 = 1;

    PPS_Mapping(65, _INPUT, _U1RX);     // RD1 = RX
    PPS_Mapping(64, _OUTPUT, _U1TX);    // RD0 = TX

    UART1_Init(baudrate);
    Delay_ms(100);
}

void InitUART3(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT, _U3RX);     // RD5 = RX
    PPS_Mapping(68, _OUTPUT, _U3TX);    // RD4 = TX

    UART3_Init(baudrate);
    Delay_ms(100);
}

void UART1_W(char *txt) {
    while (*txt)
        UART1_Write(*txt++);
}

void UART3_W(char *txt) {
    while (*txt)
        UART3_Write(*txt++);
}

void UART3_Write_Hex(unsigned char num) {
    char hex_digits[] = "0123456789ABCDEF";
    UART3_Write(hex_digits[(num >> 4) & 0x0F]);
    UART3_Write(hex_digits[num & 0x0F]);
}

void Display_TurnOn() {
    TRISG.F7 = 0;
    DISPLAY_ONOFF = 1;
}

void main() {
    char rxx;
    char response[100];
    unsigned short i = 0;
    unsigned long timeout = 0;
    unsigned short j;

    PLLFBD = 70;
    CLKDIV = 0x0000;

    Display_TurnOn();
    InitUART1(9600);
    InitUART3(9600);

    UART3_W("Sistem basladi...\r\n");
    Delay_ms(1000);

    UART3_W("Nextion'a komut gonderiliyor: get t0.txt\r\n");
    UART1_W("get t0.txt");
    UART1_Write(0xFF);
    UART1_Write(0xFF);
    UART1_Write(0xFF);

    UART3_W("UART1 verisi dinleniyor...\r\n");

    i = 0;
    timeout = 0;

    while (timeout < 10000 && i < sizeof(response) - 1) {
        if (UART1_Data_Ready()) {
            rxx = UART1_Read();

            UART3_W("Byte: ");
            UART3_Write(rxx);           // ASCII olarak yaz
            UART3_W(" [");
            UART3_Write_Hex(rxx);       // HEX olarak yaz
            UART3_W("]\r\n");

            response[i++] = rxx;
            timeout = 0;
        } else {
            Delay_ms(1);
            timeout++;
        }
    }

    response[i] = '\0';

    UART3_W("Gelen string veri:\r\n");

    for (j = 0; j < i; j++) {
        UART3_Write(response[j]);
    }
    UART3_W("\r\n");

    while (1); // dur
}
