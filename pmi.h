using namespace std;

namespace pmi {
    class Word {
    private:
        // Field variables
        string surface;
        string part_of_speech;
        string pos_detail;
    public:
        // Constructor 
        Word(string surface, string part_of_speech, string pos_detail);

        // getter
        string Surface() const;
        string PartOfSpeech() const;
        string POSDetail() const;

        // setter
        void Surface(string surface);
        void PartOfSpeech(string part_of_speech);
        void POSDetail(string pos_detail);

        // Methods
        void Print();

        // Operator
        // bool operator <(const pmi::Word& word_y);
    };

    bool operator <(const pmi::Word& word_x, const pmi::Word& word_y);
    bool operator ==(const pmi::Word& word_x, const pmi::Word& word_y);

    class SegmentedDocument {
    private:
        // Field variables
        vector<Word> words;

        // Methods

    public:
        // Constructor
        SegmentedDocument();

        // Methods
        void Print();
        void WordCount(map<string, int>* word_count);       
        void ConbinationCount(map<pair<string,string>, int>* conbination_count);
        void AddCount(map<string, int>* word_count, map<pair<string,string>, int>* conbination_count, vector<string> pos_arr);


        // setter
        void Words(vector<Word> words);
    };

    class Document {
    private:
        // Field variables
        vector<Word> words;

        // Methods
        string ParseContent(const string* file_content);
        void StoreEachWord(const string* word_pos);
        void StoreEachNoun(const string* word_pos);
    public:
        // Constructor
        Document(const string* file_content);
        vector<SegmentedDocument> Segment(int size, int shift, vector<string>);
    };

    double CalcTValue(int freq_x, int freq_y, int freq_xy, int frame_count);
    bool IsContain(string pos, vector<string>pos_arr);
} // namespace pmi
