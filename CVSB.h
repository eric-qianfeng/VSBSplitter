#ifndef CVSB_H
#define CVSB_H


class CVSB
{
    typedef struct // matching C structure
    {
        unsigned long StatusBitField;
        unsigned long StatusBitField2;
        unsigned long TimeHardware;
        unsigned long TimeHardware2;
        unsigned long TimeSystem;
        unsigned long TimeSystem2;
        unsigned char TimeStampHardwareID;
        unsigned char TimeStampSystemID;
        unsigned char NetworkID;
        unsigned char NodeID;
        unsigned char Protocol;
        unsigned char MessagePieceID;
        unsigned char ColorID;
        unsigned char NumberBytesHeader;
        unsigned char NumberBytesData;
        short DescriptionID;
        long ArbIDOrHeader;
        unsigned char Data[8];
        unsigned char AckBytes[8];
        float Value;
        unsigned char MiscData;
    } icsSpyMessage;

public:
    CVSB();

    double CalculateTimeStamp(const icsSpyMessage& msg);
    void SetTimeStamp(icsSpyMessage& msg, double newTimestamp);

};

#endif // CVSB_H
