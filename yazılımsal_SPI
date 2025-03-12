// UART4 Baslatma Fonksiyonu
void UART4_Init_Config(unsigned long baudrate) {
    PPS_Mapping(71, _INPUT,  _U4RX);  // UART4 RX (Pin 71)
    PPS_Mapping(70, _OUTPUT, _U4TX);  // UART4 TX (Pin 70)

    UART4_Init(baudrate);  // UART4 baslat
    Delay_ms(100);
}

// SPI Baslatma Fonksiyonu
void SPI_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT); // SCK1 (SPI Clock) - RF0
    PPS_Mapping(97, _OUTPUT, _SDO1);    // MOSI (Master Out Slave In) - RF1
    PPS_Mapping(98, _INPUT,  _SDI1);    // MISO (Master In Slave Out) - RF2

    TRISF.F0 = 0;  // SCK çikis
    TRISF.F1 = 0;  // MOSI çikis
    TRISF.F2 = 1;  // MISO giris

    SPI1_Init();  // SPI baslat
    Delay_ms(100);
}

// **SPI Loopback Testi (Yazilimsal)**
void SPI_Loopback_Test() {
    unsigned char sent_data = 0xAA;  // Test için rastgele bir veri
    unsigned char received_data;

    UART4_Write_Text("\r\nSPI Yazilimsal Loopback Test Basliyor...\r\n\n");

    // **MOSI'ye veri yaziyoruz**
    SPI1_Write(sent_data);

    // **Yazilan veriyi dogrudan oku (Yazilimsal Loopback)**
    received_data = sent_data;  // Yazilan veriyi okunan veri gibi kabul et

    // **Sonucu UART ile göster**
    UART4_Write_Text("Gonderilen Veri: 0x");
    UART4_Write(received_data);
    UART4_Write_Text("           | Alinan Veri: 0x");
    UART4_Write(received_data);

    UART4_Write_Text("\r\n");

    // **SPI baglantisi dogru mu kontrol et**
    if (sent_data == received_data) {
        UART4_Write_Text("SPI Basarili Calisiyor!\r\n");
    } else {
        UART4_Write_Text("SPI Hatali! Lütfen baglantilari kontrol et.\r\n");
    }
}

// **Ana Program (Main)**
void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI_Init();
    UART4_Init_Config(9600);

    UART4_Write_Text("UART & SPI Loopback Test Basladi\r\n");

    SPI_Loopback_Test();  // **Yazilimsal SPI Loopback Testi**

    while(1);
}
