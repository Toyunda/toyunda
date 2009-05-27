#ifndef H_RAWTOYUNDASTREAM
#define H_RAWTOYUNDASTREAM

class RawToyundaStream : public ToyundaSubStream
{
  Q_OBJECT;
  public:
    RawToyundaStream();
    RawToyundaStream(QString);

    void	createFromFile(const QString);
};

#endif
