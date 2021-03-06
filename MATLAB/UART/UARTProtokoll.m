function UARTProtokoll()
clc;
s = UART_init('5',9600);
while(1)
    data = fread(s,1);%readData(s);     %Datenpaket einlesen
    if ~isempty(data)    %Falls Datenpaket nicht leer
        data
%         newdata = decodeRawData(data, {'int', 'int', 'int'});
%         newdata
    end
end
UART_close(s)
end

function UART_close(s)
fclose(s);
end


function s = UART_init(comport, baud)
delete(instrfindall);
if nargin == 0
    s = serial('COM4','BaudRate',9600,'DataBits',8); %Seriellen Kanal initialisieren und �ffnen
elseif nargin == 2
    s = serial(strcat('COM',comport),'BaudRate',baud,'DataBits',8); %Seriellen Kanal initialisieren und �ffnen
end
fopen(s);
end


function outData = decodeRawData(rawData, formatArray)
    if isempty(rawData) 
        outData = [];
    else
        charlength = 1;
        intlength = 2;
        longlength = 4;
        floatlength = 4;
        doublelength = 8;

        outData = zeros(1, length(formatArray));

        cursor = 1;
        for i = 1:length(formatArray);
            switch formatArray{i}
                case 'uchar'
                    outData(i) = double(typecast(uint8(rawData(cursor)), 'uint8'));
                    cursor = cursor + charlength;
                case 'char'
                    outData(i) = double(typecast(uint8(rawData(cursor)), 'int8'));
                    cursor = cursor + charlength;
                case 'int'
                    outData(i) = double(typecast(uint8(rawData(cursor:cursor+intlength-1)), 'int16'));
                    cursor = cursor + intlength;
                case 'uint'
                    outData(i) = double(typecast(uint8(rawData(cursor:cursor+intlength-1)), 'uint16'));
                    cursor = cursor + intlength;
                case 'long'
                    outData(i) = double(typecast(uint8(rawData(cursor:cursor+longlength-1)), 'int32'));
                    cursor = cursor + longlength;
                case 'ulong'
                    outData(i) = double(typecast(uint8(rawData(cursor:cursor+longlength-1)), 'uint32'));
                    cursor = cursor + longlength;
                case 'float'
                    outData(i) = double(typecast(uint8(flipud(rawData(cursor:cursor+floatlength-1))), 'single'));
                    cursor = cursor + floatlength;
                case 'double'
                    outData(i) = typecast(uint8(flipud(rawData(cursor:cursor+doublelength-1))), 'double');
                    cursor = cursor + doublelength;
            end
        end
    end
end


function [outData] = readData(s)
outData = [];
startStopByte = 126;
escByte = 125;
waitHeader = true;
afterEsc = false;
endByte = false;

% Auf Startbyte warten
while waitHeader == true
    if s.BytesAvailable == 0    %Leeres Bytes zur�ckgeben falls keine Daten verf�gbar
        endByte = true;
        break
    else
        dataByte = fread(s,1);  %Aus der Schleife falls Startbyte gefunden
        if dataByte == startStopByte
            waitHeader = false;
        else
            warning('Kein Startbyte vorgefunden');
        end
    end
end
% Restliches Datenpaket lesen und dekodieren
while endByte ~= true
    if s.BytesAvailable == 0
    else
    dataByte = fread(s,1);  %Ein Byte aus Puffer lesen und dekodieren
        switch dataByte
            case escByte
                if afterEsc
                    outData = [outData; escByte];
                    afterEsc = false;
                else
                    afterEsc = true;
                end
            case startStopByte
                if afterEsc
                    outData = [outData; startStopByte];
                    afterEsc = false;
                else
                    endByte = true;
                end
            otherwise
                if afterEsc
                    warning('Normales Datenbyte nach ESC');
                    afterEsc = false;
                else
                    outData = [outData; dataByte];
                end
        end
    end
end
     
end

function sendData(s, array)
    startStopByte = 126;
    escByte = 125;
    fwrite(s,startStopByte);    %Startbyte senden
    for i = 1:length(array)     %Datenbytes kodieren
       switch array(i)
           case startStopByte
               fwrite(s,escByte);
               fwrite(s,startStopByte);
           case escByte
               fwrite(s,escByte);
               fwrite(s,escByte);
           otherwise 
               fwrite(s,array(i));
       end
    end
	fwrite(s,startStopByte);    %Stopbyte senden
end