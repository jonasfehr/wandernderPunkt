//
//  I_want_to_say_something.h
//  wildkleee
//
//  Created by Jonas Fehr on 13/08/2019.
//

#ifndef I_want_to_say_something_h
#define I_want_to_say_something_h

#include "ConnectedWord.h"
#include "WeightedConnection.h"

enum EnumWords : int{
    I = 0,
    WANT,
    TO,
    SAY,
    SOMETHING,
    BREAK
};


class I_want_to_say_something{
public:
    
    void setup(){
        ConnectedWord wI("I", I);
        wI.addConnection(*new WC(EnumWords::WANT, 0.5));
        wI.addConnection(*new WC(EnumWords::SAY, 0.4));
        wI.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::I, wI));
        
        ConnectedWord wWANT("WANT", WANT);
        wWANT.addConnection(*new WC(EnumWords::TO, 0.2));
        wWANT.addConnection(*new WC(EnumWords::SOMETHING, 0.5));
        wWANT.addConnection(*new WC(EnumWords::BREAK, 0.3));
        wWANT.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::WANT, wWANT));
        
        ConnectedWord wTO("TO", TO);
        wTO.addConnection(*new WC(EnumWords::SAY, 0.5));
        wTO.addConnection(*new WC(EnumWords::WANT, 0.5));
        wTO.addConnection(*new WC(EnumWords::BREAK, 0.3));
        wTO.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::TO, wTO));
        
        ConnectedWord wSAY("SAY", SAY);
        wSAY.addConnection(*new WC(EnumWords::SOMETHING, 0.5));
        wSAY.addConnection(*new WC(EnumWords::I, 0.5));
        wSAY.addConnection(*new WC(EnumWords::TO, 0.2));
        wSAY.addConnection(*new WC(EnumWords::BREAK, 0.3));
        wSAY.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::SAY, wSAY));
        
        ConnectedWord wSOMETHING("SOMETHING", SOMETHING);
        wSOMETHING.addConnection(*new WC(EnumWords::I, 0.3));
        wSOMETHING.addConnection(*new WC(EnumWords::TO, 0.2));
        wSOMETHING.addConnection(*new WC(EnumWords::BREAK, 0.3));
        wSOMETHING.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::SOMETHING, wSOMETHING));
        
        ConnectedWord wBREAK("\n", BREAK);
        wBREAK.addConnection(*new WC(EnumWords::I, 0.8));
        wBREAK.addConnection(*new WC(EnumWords::WANT, 0.5));
        wBREAK.addConnection(*new WC(EnumWords::TO, 0.2));
        wBREAK.addConnection(*new WC(EnumWords::SAY, 0.5));
        wBREAK.addConnection(*new WC(EnumWords::SOMETHING, 0.5));
        wBREAK.normalizeWeights();
        words.insert(pair<int, ConnectedWord>(EnumWords::BREAK, wBREAK));
        
        createNewCombination();
        
    }
    
    string getNextString(){
        next();
        return getString();
    }
    
    string getString(){
        return words[indexCurrentWord].getString();
    }
    
    void next(){
        indexCurrentWord++;
        if(indexCurrentWord >= combinationKeys.size()) indexCurrentWord = 0;
    }
    
    bool isNextBreak(){
        return combinationKeys[(indexCurrentWord+1)%combinationKeys.size()] == EnumWords::BREAK;
    }
    
    bool isBreak(){
        return combinationKeys[indexCurrentWord] == EnumWords::BREAK;
    }
    
    void resetIndex(){ indexCurrentWord = combinationKeys.size()-1; }
    
    int getIndex(){ return indexCurrentWord; }
    
    int getKey(){ return combinationKeys[indexCurrentWord];}
    int getKeyAtIndex(int index){
        if(index < combinationKeys.size()) return combinationKeys[index];
        else return EnumWords::BREAK;
    }
    
    
    ConnectedWord getNextWord(){
        next();
        return getWord();
    }
    
    ConnectedWord getWord(){
        return words[indexCurrentWord];
    }
    
    
    void createNewCombination(){
        combinationKeys.clear();
        for( int i = words[EnumWords::BREAK].getAdjacentWordIndex(); i != EnumWords::BREAK; i = words[combinationKeys.back()].getAdjacentWordIndex()){
            combinationKeys.push_back(i);
        }
        if(combinationKeys.back() == EnumWords::TO) combinationKeys.pop_back(); // removes TO if at the end;
        combinationKeys.push_back(EnumWords::BREAK);
        
        if(combinationKeys.size() == 1) createNewCombination();
        indexCurrentWord = combinationKeys.size()-1;
    }
    
    void shortenCombinationToIndex(int index){
        if(index < combinationKeys.size()-1 ) combinationKeys.erase(combinationKeys.begin()+index,combinationKeys.end()-1);
    }
    
    void mutate(int changeToShorten, int chanceToExtend){
        if(ofRandom(0,100)<changeToShorten && combinationKeys.size() > 2){
            if(ofRandom(0,100)>50) combinationKeys.erase(combinationKeys.begin()); // deleate first element
            else combinationKeys.erase(combinationKeys.end()-2, combinationKeys.end()-1);
        }
        
        if(ofRandom(0,100)<chanceToExtend){ // add new word to combinationKeys
            
            combinationKeys.pop_back();
            int newWord = words[combinationKeys.back()].getAdjacentWordIndex();
            while(newWord == EnumWords::BREAK){
                newWord = words[combinationKeys.back()].getAdjacentWordIndex();
            }
            combinationKeys.push_back( newWord );
            combinationKeys.push_back(EnumWords::BREAK);
        }
    }
    
    vector<int> & getCombinationKeys(){ return combinationKeys; }
    
    int getCombinationLength(){ return combinationKeys.size(); }
    
    
    string getCombinationString(){
        string output;
        for(auto & c : combinationKeys){
            if( c == EnumWords::BREAK) break;
            output = output + words[c].getString() +" ";
        }
        return output;
    }
    
    vector<int> combinationKeys;
    
    int indexCurrentWord;
    
    map<int, ConnectedWord> words;
    
};




#endif /* I_want_to_say_something_h */

