#include <iostream>
#include <string>
#include <cmath>
#include <vector>   
#include <map>
#include <algorithm>
#include <mecab.h>
#include "vital.h"
#include "pmi.h"

using namespace std;
namespace pmi {
    // Constructor 
    Word::Word(string surface, string part_of_speech, string pos_detail){
        this->Surface(surface);
        this->PartOfSpeech(part_of_speech);
        this->POSDetail(pos_detail);
    }

    // getter 
    string Word::Surface() const{
        return this->surface;
    }
    string Word::PartOfSpeech() const{
        return this->part_of_speech;
    }
    string Word::POSDetail() const{
        return this->pos_detail;
    }

    // setter
    void Word::Surface(string surface){
        this->surface = surface;
    }
    void Word::PartOfSpeech(string part_of_speech){
        this->part_of_speech = part_of_speech;
    }
    void Word::POSDetail(string pos_detail){
        this->pos_detail = pos_detail;
    }

    // Methods
    void Word::Print(){
        cout << this->Surface() << "\t" << this->PartOfSpeech() << "\t" << this->POSDetail() << endl;
    }

    // Operator
    // bool Word::operator <(const pmi::Word& word_y){
        // return this->Surface() < word_y.Surface();
    // }
    bool operator <(const pmi::Word& word_x, const pmi::Word& word_y){
        return word_x.Surface() < word_y.Surface();
    }

    bool operator ==(const pmi::Word& word_x, const pmi::Word& word_y){
        if ( (word_x.Surface() == word_y.Surface() ) 
                && ( word_x.PartOfSpeech() == word_y.PartOfSpeech() )
                && ( word_x.POSDetail() == word_y.POSDetail() ) ) {
            return true;
        } else {
            return false;
        }
    }
    // ==================================================

    // Methods
    void SegmentedDocument::Print(){
        for(vector<Word>::iterator it = this->words.begin(); it != this->words.end(); it++){
            Word word_interest = (Word)*it;
            cout << word_interest.Surface() << " ";
        }
        cout << endl;
    }

//     void SegmentedDocument::WordCount(map<string, int>* word_count){
//         for(vector<Word>::iterator it = words.begin(); it != this->words.end(); it++){
//             Word word_interest = (Word)*it;
//             // 1フレーム中に，何度同じ単語がでても，生起回数は1回．
//             if (word_interest.PartOfSpeech() == "名詞") {
//                 (*word_count)[ word_interest.Surface() ] = 1;
//             }
//         }
//     }
// 
//     void SegmentedDocument::ConbinationCount(map<pair<string,string>, int>* conbination_count){
//         // 注目単語のイテレータi
//         for(vector<Word>::iterator i = words.begin(); i != this->words.end(); i++){
//             // 注目単語の次の単語から，ペアとなる単語を探す．
//             vector<Word>::iterator i_next = i;
//             i_next++;
// 
//             // そのペアとなる単語のイテレータj
//             for(vector<Word>::iterator j = i_next; j != this->words.end(); j++){
//                 Word word_interest = (Word)*i;
//                 Word word_pair = (Word)*j;
// 
//                 if (word_interest.PartOfSpeech() == "名詞" && word_pair.PartOfSpeech() == "名詞") {
//                     // 共起回数カウント．pair[w1,w2] と pair[w2,w1] を考慮するための比較演算
//                     // 携帯が2回，電話が1回出現しても，携帯と電話の共起回数は1回
//                     if (word_interest.Surface() < word_pair.Surface()) {
//                         (*conbination_count)[make_pair(word_interest.Surface(), word_pair.Surface() )] = 1;
//                     } else {
//                         (*conbination_count)[make_pair(word_pair.Surface(), word_interest.Surface() )] = 1;
//                     }
//                 }
//             }
//         }
//     }

    void SegmentedDocument::AddCount(map<string, int>* word_count, map<pair<string,string>, int>* conbination_count, vector<string>pos_arr){

        // 重複を取り除く
        vector<Word> unique_words = this->words;
        sort( unique_words.begin(), unique_words.end() );
        vector<Word>::iterator end_it = unique( unique_words.begin(), unique_words.end() );
        unique_words.erase(end_it, unique_words.end() );

        // 注目単語のイテレータi
        for(vector<Word>::iterator i = unique_words.begin(); i != unique_words.end(); i++){
            // 生起回数カウント
            Word word_interest = (Word)*i;
            if ( IsContain( word_interest.PartOfSpeech(), pos_arr) ) {
                (*word_count)[ word_interest.Surface() ] += 1;
            }

            // 共起回数カウント
            // 注目単語の次の単語から，ペアとなる単語を探す．
            vector<Word>::iterator i_next = i;
            i_next++;

            // そのペアとなる単語のイテレータj
            for(vector<Word>::iterator j = i_next; j != unique_words.end(); j++){
                Word word_pair = (Word)*j;

                if ( IsContain( word_interest.PartOfSpeech(), pos_arr) && IsContain( word_pair.PartOfSpeech(), pos_arr) ) {
                    // 共起回数カウント．pair[w1,w2] と pair[w2,w1] を考慮するための比較演算
                    if (word_interest.Surface() < word_pair.Surface()) {
                        (*conbination_count)[make_pair(word_interest.Surface(), word_pair.Surface() )] += 1;
                    } else {
                        (*conbination_count)[make_pair(word_pair.Surface(), word_interest.Surface() )] += 1;
                    }
                }
            }
        }
    }

    // setter
    void SegmentedDocument::Words(vector<Word> words){
        this->words = words;
    }

    // Constructor
    SegmentedDocument::SegmentedDocument(){
        
    }

    // ==================================================

    // Methods
    string Document::ParseContent(const string* file_content){
        MeCab::Tagger* parser = MeCab::createTagger("--node-format=%m\\t%f[0]\\t%f[1]\\n --eos-format='' ");
        string result = parser->parse(file_content->c_str() );        // docをchar*に変換して，パースする

        return result;
    }

    void Document::StoreEachWord(const string* word_pos){
        vector<string> elements = vital::split(*word_pos, '\t');

        // 意図しない文字列が来た場合に備え，表層，品詞，品詞詳細があるか確認
        if (elements.size() == 3) {
            // 表層単語の格納
            vector<string>::iterator it = elements.begin();
            string surface = (string)*it;

            // 品詞の格納
            it++;
            string part_of_speech = (string)*it;

            // 品詞の詳細の格納
            it++;
            string pos_detail = (string)*it;

            Word word(surface, part_of_speech, pos_detail);

            this->words.push_back(word);
        }
    }

    // 文書の順序的名詞リストを返す
    void Document::StoreEachNoun(const string* word_pos){
        vector<string> elements = vital::split(*word_pos, '\t');

        // 意図しない文字列が来た場合に備え，表層，品詞，品詞詳細があるか確認
        if (elements.size() == 3) {
            // 表層単語の格納
            vector<string>::iterator it = elements.begin();
            string surface = (string)*it;

            // 品詞の格納
            it++;
            string part_of_speech = (string)*it;

            // 品詞の詳細の格納
            it++;
            string pos_detail = (string)*it;

            if (part_of_speech == "名詞") {
                Word word(surface, part_of_speech, pos_detail);
                this->words.push_back(word);
            }
        }      
    }
    // ==================================================
    // フレーム化の実質的実装部分
    // ==================================================
    vector<SegmentedDocument> Document::Segment(const int size, const int shift, vector<string> content_poslist){
        vector<SegmentedDocument> seg_doc_arr;
        vector<Word> segment;
        int noun_count = 0;
        vector<Word>::iterator next_begin = this->words.begin();

        for(vector<Word>::iterator it = this->words.begin(); it != this->words.end(); it++){
            // 注目要素抽出
            Word word_interest = (Word)*it;

            // 要素のプッシュ
            segment.push_back(word_interest);

            // 内容語であると指定した品詞かどうか判定．指定した品詞の場合，カウント++
            // if (word_interest.PartOfSpeech() == "名詞") {
            if ( IsContain(word_interest.PartOfSpeech(), content_poslist) ) {
                noun_count++;
            }

            // フレームシフト分の内容語を見つけたら，次のフレーム開始場所を記憶
            if (noun_count == shift) {
                next_begin = it;
            }

            // 1フレーム分の内容語を見つけたら，フレームをプッシュして，また作りなおす．
            else if (noun_count == size) {
                // 
//                 for(vector<Word>::iterator iit = segment.begin(); it != segment.end(); it++){
//                     Word word = (Word)*iit;
//                     cout << word.Surface() << ends;
//                 }
//                 cout << endl;


                // 1フレーム完成
                SegmentedDocument seg_doc;
                seg_doc.Words(segment);

                // フレームをプッシュ
                seg_doc_arr.push_back(seg_doc);

                // バッファをクリア
                segment.clear();

                // 次のフレーム開始場所から，また作りなおす．
                it = next_begin;
                noun_count = 0;
            }

            // 文書の末尾まで探索したら，とりあえずプッシュする
            if (it == --words.end() ) {
                // 1フレーム完成
                SegmentedDocument seg_doc;
                seg_doc.Words(segment);

                // フレームをプッシュ
                seg_doc_arr.push_back(seg_doc);
            }
        }

        return seg_doc_arr;
    }


    // Constructor
    Document::Document(const string* file_content){
        string parsed_content = this->ParseContent(file_content);
        vector<string> word_pos_list = vital::split(parsed_content, '\n');
        for (vector<string>::iterator it = word_pos_list.begin(); it != word_pos_list.end(); it++) {
            string line = (string)*it;
            // this->StoreEachNoun(&line);
            this->StoreEachWord(&line);
        }
    }


    // PMI method
    double CalcTValue(int freq_x, int freq_y, int freq_xy, int frame_count){
        return (abs(freq_xy - ((double)( freq_x * freq_y ) / (double)frame_count))) / (sqrt((double)freq_xy));
    }

    // 
    bool IsContain(string pos, vector<string>pos_arr){
        vector<string>::iterator findit = find(pos_arr.begin(), pos_arr.end(), pos);
        if (findit != pos_arr.end()) {
            return true;
        }
        else {
            return false;
        }
    }


}// namespace pmi
