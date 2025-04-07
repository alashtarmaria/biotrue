// SPI ve UART Baslatma
void SPI_UART_Init() {
    PPS_Mapping(71, _INPUT,  _U4RX);  // UART4 RX (Pin 71)
    PPS_Mapping(70, _OUTPUT, _U4TX);  // UART4 TX (Pin 70)

    UART4_Init(9600);  // UART4 baslat
    Delay_ms(100);

    // SPI PPS Tanimlamalari
    PPS_Mapping(96, _OUTPUT, _SCK1OUT); // SCK1 (SPI Clock) - RF0
    PPS_Mapping(97, _OUTPUT, _SDO1);    // MOSI (Master Out Slave In) - RF1
    PPS_Mapping(98, _INPUT,  _SDI1);    // MISO (Master In Slave Out) - RF2

    SPI1_Init(); // SPI baslat
}

// SPI Loopback Testi: MOSI ve MISO Degerlerini Okuma
void SPI_Loopback_Test() {
    unsigned char test_data = 0xAA; // Test için rastgele bir veri
    unsigned char received_data;

    UART4_Write_Text("\r\nSPI MISO/MOSI Test Baslatiliyor...\r\n");

    // SPI'ye test verisini gönder ve MISO'dan gelen veriyi oku
    received_data = SPI1_Read(test_data);

    // MOSI'de gönderilen veriyi göster
    UART4_Write_Text("MOSI Gönderilen Veri: 0x");
    UART4_Write(test_data);
    UART4_Write_Text("\r\n");

    // MISO'dan okunan veriyi göster
    UART4_Write_Text("MISO Okunan Veri: 0x");
    UART4_Write(received_data);
    UART4_Write_Text("\r\n");

    // Dogrulama yapalim
    if (test_data == received_data) {
        UART4_Write_Text("? SPI Loopback Testi Basarili! Veriler eslesti.\r\n");
    } else {
        UART4_Write_Text("? SPI Hata! MOSI ve MISO eslesmedi.\r\n");
    }
}

// **Ana Program (Main)**
void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI_UART_Init();  // SPI ve UART'i baslat
    UART4_Write_Text("UART & SPI Loopback Test Basladi\r\n");

    while(1) {
        SPI_Loopback_Test(); // SPI loopback testini sürekli çalistir
        Delay_ms(1000);  // Her 1 saniyede bir testi tekrarla
    }
}
