//
//  ConnectedWord.h
//  wildkleee
//
//  Created by Jonas Fehr on 13/08/2019.
//

#ifndef ConnectedWord_h
#define ConnectedWord_h

#include "WeightedConnection.h"
#include "SvgLoader.h"


class ConnectedWord{
public:
    ConnectedWord(){};
    
    ConnectedWord(string word, int index){
        this->word = word;
        this->index = index;
    }
    

    
    void addConnection(WC connection){
        connections.push_back(connection);
        normalizeWeights();
    }
    
    void normalizeWeights(){
        float sumWeights = 0;
        for(auto & c : connections){
            sumWeights += c.weight;
        }
        for(auto & c : connections){
            c.weightNormalised = c.weight/sumWeights;
        }
        calculateWeightRanges();
    }
    
    void calculateWeightRanges(){
        connections[0].from = 0;
        connections[0].to = connections[0].weightNormalised;
        for(int i = 1; i < connections.size(); i++){
            connections[i].from = connections[i-1].to;
            connections[i].to = connections[i].from + connections[i].weightNormalised;
        }
    }
    
    string getString(){
        return word;
    }
    
    int getAdjacentWordIndex(){
        float rndF = ofRandom(0., 1.);
        for(auto & c : connections){
            if(c.from < rndF && c.to > rndF){
                return c.getConnected();
            }
        }
        return ;
    }
    
    
    vector<WC> & getConnections(){ return connections; }
    
    
    string word;
    int index;
    
    vector<WC> connections;
};

#endif /* Word_h */
