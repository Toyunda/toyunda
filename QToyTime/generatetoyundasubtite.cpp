#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QThread>
#include <unistd.h>

bool    generateToyundaSubtitle(QString lyr, QString frm, QString outFile)
{
    QFile file(outFile);
    QRegExp lyrLineRegex("[^\n#]+&[^\n]+");
    QRegExp frmLineRegex("\\d+\\s\\d+");

    if (lyr.isEmpty() || frm.isEmpty())
        return false;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can't open file : " << outFile << " - "<<  file.errorString();
        return false;
    }
    QTextStream out(&file);
    out.setCodec("ISO 8859-1");

    QTextStream lyrStream(&lyr);
    QTextStream frmStream(&frm);

    int nbPipe = 1;
    int marging_start = 30;
    int marging_end = 20;
    int dispoLine[4];
    for (int i = 0; i < 4; i++)
        dispoLine[i] = -1;
    while(!lyrStream.atEnd() && !frmStream.atEnd())
    {
        QString lyrLine = lyrStream.readLine();
        if (lyrLineRegex.exactMatch(lyrLine))
        {
            int start, end;
            bool    first = true;
            int lyrFound = 0;
            int lyrPos = -1;
            int linePos = 0;
            QString rawLyrLine = lyrLine;
            rawLyrLine.remove("&");
            while ((lyrPos = lyrLine.indexOf("&", lyrPos + 1)) >= 0)
            {
                QString frmLine;
                do {
                    frmLine = frmStream.readLine();
                } while(!frmLineRegex.exactMatch(frmLine) && !frmStream.atEnd());
                if (frmStream.atEnd())
                    break;
                int s;
                s = frmLine.split(" ").first().toInt();
                end = frmLine.split(" ").last().toInt();
                if (first)
                {
                    start = s;
                    first = false;
                    for (int i = 0; i < 4; i++)
                    {
                        if (dispoLine[i] < start - marging_start)
                        {
                            dispoLine[i] = -1;
                            nbPipe = i * 3 + 1;
                            linePos = i;
                            break;
                        }
                    }
                }
                int nbSpaceBefore = lyrPos - lyrFound;
                int  nbSpaceAfter = rawLyrLine.size() - nbSpaceBefore - 1;
                out << QString("{%1}{%2}%3%4").arg(s)
                       .arg(end)
                       .arg(QString().fill('|', nbPipe))
                       .arg(QString().fill(' ', nbSpaceBefore));
                out << QChar(0xFF) << QString().fill(' ', nbSpaceAfter) << "\n";
                lyrFound++;
            }
            dispoLine[linePos] = end + marging_end;
            out << QString("{%1}{%2}%3%4\n").arg((start - marging_start >= 0 ) ? start - marging_start : 0)
                   .arg(end + marging_end).arg(QString().fill('|', linePos * 3))
                   .arg(rawLyrLine);
        }
    }
    return true;
}

