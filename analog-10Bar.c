char buffer[64];

// **UART Baslatma**
void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);
    PPS_Mapping(68, _OUTPUT, _U3TX);
    UART3_Init(baudrate);
    Delay_ms(100);
}

// SPI PPS Konfigürasyonu
void SPI1_PPS_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT);  // SPI Clock (SCK) -> RP96 (RF0)
    PPS_Mapping(97, _OUTPUT, _SDO1);     // SPI MOSI (Master Out Slave In) -> RP97 (RF1)
    PPS_Mapping(98, _INPUT, _SDI1);      // SPI MISO (Master In Slave Out) -> RP98 (RF2)
    PPS_Mapping(104, _OUTPUT, _SS1OUT);  // SPI CS (Chip Select) -> RP104 (RF8)

    TRISF.F0 = 0; // SCK (Clock) çikis
    TRISF.F1 = 0; // MOSI (Data Out) çikis
    TRISF.F2 = 1; // MISO (Data In) giris
    TRISF.F8 = 0; // CS (Chip Select) çikis
    LATF.F8 = 1;  // SS'yi HIGH yap (Bastaki default durum)
}

// SPI Baslatma
void SPI1_Initialize_Master() {
    SPI1_Init();
}

// ADC Okuma Fonksiyonu (MCP3208)
unsigned int Read_ADC_Channel(unsigned char channel) {
    unsigned char msb, lsb;
    unsigned int adcValue;
    unsigned char command;
    unsigned char channelBits = channel << 6;

    if (channel > 3)
        command = 0b00000111;
    else
        command = 0b00000110;

    LATF.F8 = 0;
    SPI1_Write(command);
    msb = SPI1_Read(channelBits);
    lsb = SPI1_Read(0x00);
    LATF.F8 = 1;

    msb = msb & 0b00001111;
    adcValue = (msb << 8) | lsb;
    return adcValue;
}

// Sensör Çikisina Göre Basinç Okuma ve UART ile Gönderme
void Read_Pressure() {
    unsigned int adcRaw;
    float Vadc, Pressure;
    char txt[32];

    adcRaw = Read_ADC_Channel(0);  // **CH0'dan ADC okuma (SPA_TP1 dogrudan bagli!)**
    Vadc = (adcRaw / 4096.0) * 4.096;  // **ADC'yi voltaja çevir (MCP3208 12-bit, 4.096V referansli)**

    // **DÜZELTILMIS BASINÇ FORMÜLÜ**
    Pressure = ((Vadc - 0.5) / 4) * 10;

    // **UART ile verileri gönder**
    sprintf(txt, "sensor output: %u\r\n", adcRaw);
    UART3_Write_Text(txt);


    Delay_ms(1000);
}

// **Ana Program (Main)**
void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI1_PPS_Init();
    SPI1_Initialize_Master();
    UART3_Init_Config(9600);

    UART3_Write_Text("Okuma Basladi...\r\n");

    while (1) {
        Read_Pressure();
    }
}
