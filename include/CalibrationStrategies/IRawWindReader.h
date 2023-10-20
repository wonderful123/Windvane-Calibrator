class IRawWindReader {
public:
    virtual ~IRawWindReader() = default;

    // Read and return the raw wind direction.
    virtual double readRawWindDirection() = 0;
};