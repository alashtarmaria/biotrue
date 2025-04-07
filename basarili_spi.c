void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);  // UART3 RX
    PPS_Mapping(68, _OUTPUT, _U3TX);  // UART3 TX

    UART3_Init(baudrate);
    Delay_ms(100);
}

// **SPI Baslatma Fonksiyonu**
void SPI1_Custom_Init() {
    SPI1STATbits.SPIEN = 0;  // SPI'yi kapat

    SPI1CON1 = 0x0130; // Master Mode, Fosc/128, Idle Low, Sample Middle
    SPI1STATbits.SPIROV = 0; // Overflow bayragini temizle
    SPI1STATbits.SPIEN = 1;  // SPI'yi aktif et
}

// **SPI PPS Ayarlari**
void SPI_PPS_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT);
    PPS_Mapping(97, _OUTPUT, _SDO1);
    PPS_Mapping(98, _INPUT,  _SDI1);
    PPS_Mapping(112, _OUTPUT, _SS1OUT);

    TRISF.F0 = 0; // SCK çikis
    TRISF.F1 = 0; // MOSI çikis
    TRISF.F2 = 1; // MISO giris
    TRISG.F0 = 0; // SS çikis
    LATG.F0 = 1;  // **SS'i HIGH yap**
}

// **SPI Veri Gönderme ve Alma Fonksiyonu**
unsigned char SPI1_Transfer(unsigned char sendByte) {
    SPI1BUF = sendByte;
    while (!SPI1STATbits.SPIRBF); // Veri alimi tamamlanana kadar bekle
    return SPI1BUF; // Gelen veriyi döndür
}

// **SPI Testi: UART ile Giden ve Gelen Veriyi Göster**
void SPI_UART_Test() {
    unsigned char sent_byte = 0xAA;  // Test için 0xAA yolla
    unsigned char received_byte;

    UART3_Write_Text("\r\nSPI Test Basladi...\r\n");

    LATG.F0 = 0; // **SS LOW**
    received_byte = SPI1_Transfer(sent_byte);
    LATG.F0 = 1; // **SS HIGH**

    // **UART Üzerinden Göster**
    UART3_Write_Text("Gonderilen Veri: 0x");
    UART3_Write(sent_byte);
    UART3_Write_Text("\r\n");

    UART3_Write_Text("Okunan Veri: 0x");
    UART3_Write(received_byte);
    UART3_Write_Text("\r\n");

    if (sent_byte == received_byte) {
        UART3_Write_Text("SPI Basarili: Veri Dogru Geldi.\r\n");
    } else {
        UART3_Write_Text("SPI Hatali: Veri Eslesmedi!\r\n");
    }
}

// **Ana Program (Main)**
void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI_PPS_Init();
    SPI1_Custom_Init();
    UART3_Init_Config(9600);

    UART3_Write_Text("UART3 & SPI Test Basladi...\r\n");

    while(1) {
        SPI_UART_Test();
        Delay_ms(1000);
    }
}
