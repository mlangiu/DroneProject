function UARTProtokollLeser()
clear all;
delete(instrfindall);
s = serial('COM3','BaudRate',9600,'DataBits',8); %Seriellen Kanal initialisieren und �ffnen
fopen(s);
% while(1)
%     data = readData(s);     %Datenpaket einlesen
%     if ~isempty(data)    %Falls Datenpaket nicht leer
%             newdata = double([typecast(uint8(flipud(data(1:2))), 'int16');...   %Jeweils LSB und MSB von drei Int zusammenf�gen
%             typecast(uint8(flipud(data(5:6))), 'int16');...
%             typecast(uint8(flipud(data(3:4))), 'int16')]);
%             bar(newdata);   %Daten plotten
%             ylim([-800 800]);
%     end
%     pause(0.05);
% end
fclose(s);
hold off;
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