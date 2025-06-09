#pragma once

#include <vector>
#include <algorithm>
#include <utility>

#include <Graph.hpp>

namespace bgl::opt
{
    inline size_t INVALID = -1;

    class Vf2ppOrder
    {
    public:
        //The graph to be embedded
        Graph &_g1;

        //The graph into which g1 is to be embedded
        Graph &_g2;

        //_order[i] is a node of g1 for which a pair is searched if depth=i
        std::vector<node> _order;

        //_rNewLabels1[v] is a pair of form
        //(label; num. of uncov. nodes with such label and no covered neighbours)
        graph_node_map<std::vector<std::pair<int,int>>> _rNewLabels1;

        //_rInOutLabels1[v] is the number of covered neighbours of v for each label
        //in form (label,number of such labels)
        graph_node_map<std::vector<std::pair<int,int>>> _rInOutLabels1;

        //_intLabels1[v]==i means that node v has label i in _g1
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        vertex_map _intLabels1;

        //_intLabels2[v]==i means that node v has label i in _g2
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        vertex_map _intLabels2;

        //_intLabelsEdge1[e]==i means that edge e has label i in _g1
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        edge_map _intLabelsEdge1;

        //_intLabelsEdge2[v]==i means that edge e has label i in _g2
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        edge_map _intLabelsEdge2;

        //lookup tables for manipulating with label class cardinalities
        //(after use they have to be reset to 0..0)
        std::vector<int> _labelTmp1,_labelTmp2;

        //largest label
        int _maxLabel;

        void processBfsTree(node source, unsigned int& orderIndex,
        graph_node_map<int>& dm1,
        graph_node_map<bool>& added)
        {
            _order[orderIndex]=source;
            added[source]=1;

            unsigned int endPosOfLevel=orderIndex,
            startPosOfLevel=orderIndex,
            lastAdded=orderIndex;

            graph_node_map<int> currConn(_g1,0);

            while(orderIndex<=lastAdded)
            {
                node currNode = _order[orderIndex];
                for(IncEdgeIt e = _g1.edge_iterator(currNode); e.isValid(); ++e)
                {
                    node n = _g1.oppositeNode(currNode, e);
                    if(!added[n])
                    {
                        _order[++lastAdded]=n;
                        added[n]=1;
                    }
                }
                if(orderIndex>endPosOfLevel)
                {
                    for(unsigned int j = startPosOfLevel; j <= endPosOfLevel; ++j)
                    {
                        int minInd=j;
                        for(unsigned int i = j+1; i <= endPosOfLevel; ++i)
                            if(currConn[_order[i]]>currConn[_order[minInd]]||
                            (currConn[_order[i]]==currConn[_order[minInd]]&&
                            (dm1[_order[i]]>dm1[_order[minInd]]||
                            (dm1[_order[i]]==dm1[_order[minInd]]&&
                            _labelTmp1[_intLabels1[_order[minInd]]]>
                            _labelTmp1[_intLabels1[_order[i]]]))))
                                minInd=i;

                        --_labelTmp1[_intLabels1[_order[minInd]]];
                        for(IncEdgeIt e = _g1.edge_iterator(_order[minInd]); e.isValid(); ++e)
                            ++currConn[_g1.oppositeNode(_order[minInd], e)];
                        
                        std::swap(_order[j],_order[minInd]);
                    }
                    startPosOfLevel=endPosOfLevel+1;
                    endPosOfLevel=lastAdded;
                }
                ++orderIndex;
            }
        }

        //we will find pairs for the nodes of g1 in this order
        void initOrder()
        {
            for(NodeIt n2 = _g2.node_iterator(); n2.isValid(); ++n2)
                ++_labelTmp1[_intLabels2[*n2]];

            graph_node_map<int> dm1(_g1,0);
            for(EdgeIt e = _g1.all_edge_iterator(); e.isValid(); ++e)
            {
                ++dm1[_g1.u(e)];
                ++dm1[_g1.v(e)];
            }

            graph_node_map<bool> added(_g1, 0);
            unsigned int orderIndex=0;

            for(NodeIt n = _g1.node_iterator(); n.isValid();)
            {
                if(!added[*n])
                {
                    node minNode = *n;
                    for(NodeIt n1 = _g1.node_iterator(n); n1.isValid(); ++n1)
                        if(!added[*n1] &&
                            (_labelTmp1[_intLabels1[minNode]] > _labelTmp1[_intLabels1[*n1]] ||
                                (dm1[minNode] < dm1[*n1] && _labelTmp1[_intLabels1[minNode]] == _labelTmp1[_intLabels1[*n1]])
                            )
                        )
                            minNode=*n1;
                    processBfsTree(minNode,orderIndex,dm1,added);
                }
                else
                    ++n;
            }
            for(unsigned int i = 0; i < _labelTmp1.size(); ++i)
                _labelTmp1[i]=0;
        }

        int getMaxLabel() const
        {
            int m = -1;
            for(NodeIt n1 = _g1.node_iterator(); n1.isValid(); ++n1)
            {
                const int& currIntLabel = _intLabels1[*n1];
                if(currIntLabel > m)
                    m = currIntLabel;
            }
            for(NodeIt n2 = _g2.node_iterator(); n2.isValid(); ++n2)
            {
                const int& currIntLabel = _intLabels2[*n2];
                if(currIntLabel > m)
                    m = currIntLabel;
            }
            return m;
        }

        //calculate the lookup table for cutting the search tree
        void initRNew1tRInOut1t()
        {
            graph_node_map<int> tmp(_g1,0);
            for(unsigned int i=0; i<_order.size(); ++i)
            {
                tmp[_order[i]] = -1;
                for(IncEdgeIt e1 = _g1.edge_iterator(_order[i]); e1.isValid(); ++e1)
                {
                    const node currNode = _g1.oppositeNode(_order[i], e1);
                    
                    if(tmp[currNode] > 0)
                        ++_labelTmp1[_intLabels1[currNode]];
                    else if(tmp[currNode] == 0)
                        ++_labelTmp2[_intLabels1[currNode]];
                }
                //_labelTmp1[i]=number of neightbours with label i in set rInOut
                //_labelTmp2[i]=number of neightbours with label i in set rNew
                for(IncEdgeIt e1 = _g1.edge_iterator(_order[i]); e1.isValid(); ++e1)
                {
                    const int& currIntLabel = _intLabels1[_g1.oppositeNode(_order[i], e1)];
                    if(_labelTmp1[currIntLabel] > 0)
                    {
                        _rInOutLabels1[_order[i]].push_back(std::make_pair(currIntLabel, _labelTmp1[currIntLabel]));
                        _labelTmp1[currIntLabel] = 0;
                    }
                    else if(_labelTmp2[currIntLabel] > 0)
                    {
                        _rNewLabels1[_order[i]].push_back(std::make_pair(currIntLabel, _labelTmp2[currIntLabel]));
                        _labelTmp2[currIntLabel] = 0;
                    }
                }

                for(IncEdgeIt e1 = _g1.edge_iterator(_order[i]); e1.isValid(); ++e1)
                {
                    int& tmpCurrNode=tmp[_g1.oppositeNode(_order[i], e1)];
                    if(tmpCurrNode!=-1)
                        ++tmpCurrNode;
                }
            }
        }

        ///Constructor
        Vf2ppOrder(Graph& g1, Graph& g2) :
        _g1(g1), _g2(g2), _order(g1.node_count(), INVALID),
        _rNewLabels1(_g1),_rInOutLabels1(_g1), 
        _intLabels1(g1.get_node_map()) ,_intLabels2(g2.get_node_map()),
        _intLabelsEdge1(g1.get_edge_map()), _intLabelsEdge2(g2.get_edge_map())
        {
            _maxLabel = getMaxLabel();
            _labelTmp1 = std::vector<int>(_maxLabel+1);
            _labelTmp2 = std::vector<int>(_maxLabel+1);
            initOrder();

            initRNew1tRInOut1t();
        }

        std::vector<node>& get_order()
        {
            return _order;
        }

        int get_maxLabel() {return _maxLabel;}

        ///Destructor
        ~Vf2ppOrder()
        {
            ;
        }
    };

    class Vf2pp
    {
    private:
        //The graph to be embedded
        Graph &_g1;

        //The graph into which g1 is to be embedded
        Graph &_g2;

        //Current depth in the search tree
        int _depth;

        //The current mapping. _mapping[v1]=v2 iff v1 has been mapped to v2,
        //where v1 is a node of G1 and v2 is a node of G2
        std::vector<node> &_mapping;

        //_order[i] is a node of g1 for which a pair is searched if depth=i
        std::vector<node>& _order;

        //_conn[v2] = number of covered neighbours of v2
        graph_node_map<int> _conn;

        //_currEdgeIts[i] is the last used edge iterator in the i-th
        //depth to find a pair for node _order[i]
        std::vector<IncEdgeIt> _currEdgeIts;

        //_rNewLabels1[v] is a pair of form
        //(label; num. of uncov. nodes with such label and no covered neighbours)
        graph_node_map<std::vector<std::pair<int,int>>>& _rNewLabels1;

        //_rInOutLabels1[v] is the number of covered neighbours of v for each label
        //in form (label,number of such labels)
        graph_node_map<std::vector<std::pair<int,int>>>& _rInOutLabels1;

        //_intLabels1[v]==i means that node v has label i in _g1
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        vertex_map _intLabels1;

        //_intLabels2[v]==i means that node v has label i in _g2
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        vertex_map _intLabels2;

        //_intLabelsEdge1[e]==i means that edge e has label i in _g1
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        edge_map _intLabelsEdge1;

        //_intLabelsEdge2[v]==i means that edge e has label i in _g2
        //(i is in {0,1,2,..,K-1}, where K is the number of diff. labels)
        edge_map _intLabelsEdge2;

        //lookup tables for manipulating with label class cardinalities
        //(after use they have to be reset to 0..0)
        std::vector<int> _labelTmp1,_labelTmp2;

        int _maxLabel;

        //improved cutting function
        bool cutByLabels(const node n1,const node n2)
        {
            for(IncEdgeIt e2 = _g2.edge_iterator(n2); e2.isValid(); ++e2)
            {
                const node currNode = _g2.oppositeNode(n2, e2);
                if(_conn[currNode] > 0) 
                    --_labelTmp1[_intLabels2[currNode]];
                else if(_conn[currNode] == 0)
                    --_labelTmp2[_intLabels2[currNode]];
            }

            bool ret = 1;
            if(ret)
            {
                for(unsigned int i = 0; i < _rInOutLabels1[n1].size(); ++i)
                    _labelTmp1[_rInOutLabels1[n1][i].first]+=_rInOutLabels1[n1][i].second;

                for(unsigned int i = 0; i < _rNewLabels1[n1].size(); ++i)
                    _labelTmp2[_rNewLabels1[n1][i].first]+=_rNewLabels1[n1][i].second;

                for(unsigned int i = 0; i < _rInOutLabels1[n1].size(); ++i)
                    if(_labelTmp1[_rInOutLabels1[n1][i].first]>0)
                    {
                        ret = 0;
                        break;
                    }
                
                if(ret)
                    for(unsigned int i = 0; i < _rNewLabels1[n1].size(); ++i)
                        if(_labelTmp2[_rNewLabels1[n1][i].first]>0)
                        {
                            ret = 0;
                            break;
                        }

                for(unsigned int i = 0; i < _rInOutLabels1[n1].size(); ++i)
                    _labelTmp1[_rInOutLabels1[n1][i].first]=0;

                for(unsigned int i = 0; i < _rNewLabels1[n1].size(); ++i)
                    _labelTmp2[_rNewLabels1[n1][i].first]=0;
            }

            for(IncEdgeIt e2 = _g2.edge_iterator(n2); e2.isValid(); ++e2)
            {
                
                const node currNode=_g2.oppositeNode(n2, e2);
                _labelTmp1[_intLabels2[currNode]]=0;

                if(_conn[currNode]==0)
                    _labelTmp2[_intLabels2[currNode]]=0;
            }

            return ret;
        }

        //try to exclude the matching of n1 and n2
        bool feas(const node n1,const node n2)
        {
            if(_intLabels1[n1]!=_intLabels2[n2]) return 0;

            // have to this because boost does not 
            // work in the same way as lemon :(
            for(IncEdgeIt e1 = _g1.edge_iterator(n1); e1.isValid(); ++e1)
            {
                volatile const node currNode = _g1.oppositeNode(n1, *e1);
                if(_mapping[currNode]!=INVALID)
                    _conn[_mapping[currNode]] -= _intLabelsEdge1[_g1.getEdge(n1, currNode)] + 1;
            }

            bool isIso = 1;
            for(IncEdgeIt e2 = _g2.edge_iterator(n2); e2.isValid(); ++e2)
            {
                auto edge = *e2;
                volatile const node currNode = _g2.oppositeNode(n2, *e2);
                int& connCurrNode = _conn[_g2.oppositeNode(n2, edge)];
                if(connCurrNode<-1)
                {
                    connCurrNode += _intLabelsEdge2[_g2.getEdge(n2, currNode)] + 1;
                    // The edge weights are not the same, not isomorphic
                    if(connCurrNode != -1)
                    {
                        isIso = 0;
                        break;
                    }
                }
                else if(connCurrNode==-1) {
                    isIso=0;
                    break;
                }
            }

            if(isIso)
                for(IncEdgeIt e1 = _g1.edge_iterator(n1); e1.isValid(); ++e1)
                {
                    const node& currNodePair = _mapping[_g1.oppositeNode(n1, e1)];
                    int& connCurrNodePair=_conn[currNodePair];
                    if(currNodePair != INVALID && connCurrNodePair!=-1)
                    {
                        isIso=0;
                        connCurrNodePair=-1;
                    }
                }
            else
                for(IncEdgeIt e1 = _g1.edge_iterator(n1); e1.isValid(); ++e1)
                {
                    const node currNode = _mapping[_g1.oppositeNode(n1, e1)];
                    if(currNode != INVALID)
                        _conn[currNode]=-1;
                }

            return isIso && cutByLabels(n1,n2);
        }

        //maps n1 to n2
        void addPair(const node n1,const node n2)
        {
            _conn[n2] = -1;
            _mapping[n1] = n2;
            for(IncEdgeIt e2 = _g2.edge_iterator(n2); e2.isValid(); ++e2)
            {
                int& currConn = _conn[_g2.oppositeNode(n2, e2)];
                if(currConn!=-1)
                    ++currConn;
            }
        }

        //removes mapping of n1 to n2
        void subPair(const node n1,const node n2)
        {
            _conn[n2]=0;
            _mapping[n1] = INVALID;
            for(IncEdgeIt e2 = _g2.edge_iterator(n2); e2.isValid(); ++e2)
            {
                int& currConn = _conn[_g2.oppositeNode(n2, e2)];
                if(currConn>0)
                    --currConn;
                else if(currConn==-1)
                    ++_conn[n2];
            }
        }

        bool extMatch()
        {
            bool is_first_candidate = true;
            while(_depth>=0)
            {
                if(_depth==static_cast<int>(_order.size()))
                {
                    //all nodes of g1 are mapped to nodes of g2
                    --_depth;
                    return true;
                }
                node& nodeOfDepth = _order[_depth];
                const node& pairOfNodeOfDepth = _mapping[nodeOfDepth];
                IncEdgeIt &edgeItOfDepth = _currEdgeIts[_depth];
                //the node of g2 whose neighbours are the candidates for
                //the pair of _order[_depth]
                node currPNode;
                if(!edgeItOfDepth.isValid())
                {
                    IncEdgeIt fstMatchedE = _g1.edge_iterator(nodeOfDepth);
                    //if _mapping[_order[_depth]]!=INVALID, we don't need fstMatchedE
                    if(pairOfNodeOfDepth == INVALID && !is_first_candidate)
                    {
                        //find fstMatchedE, it could be preprocessed
                        for(; fstMatchedE.isValid() && _mapping[_g1.oppositeNode(nodeOfDepth, fstMatchedE)] == INVALID; ++fstMatchedE); 
                    }
                    if(!fstMatchedE.isValid() || pairOfNodeOfDepth != INVALID || is_first_candidate)
                    {
                        is_first_candidate = false;
                        //We found no covered neighbours, this means that
                        //the graph is not connected (or _depth==0). Each
                        //uncovered (and there are some other properties due
                        //to the spec. problem types) node of g2 is
                        //candidate. We can read the iterator of the last
                        //tried node from the match if it is not the first
                        //try (match[nodeOfDepth]!=INVALID)
                        NodeIt n2 = _g2.node_iterator();
                        //if it's not the first try
                        if(pairOfNodeOfDepth!=INVALID)
                        {
                            n2 = ++_g2.node_iterator(pairOfNodeOfDepth);
                            subPair(nodeOfDepth,pairOfNodeOfDepth);
                        }
                        for(; n2.isValid(); ++n2)
                        {
                            if(_conn[*n2]==0 && feas(nodeOfDepth,*n2))
                                break;
                        }
                        // n2 is the next candidate
                        if(n2.isValid())
                        {
                            addPair(nodeOfDepth,*n2);
                            ++_depth;
                        }
                        else // there are no more candidates
                            --_depth;
                        continue;
                    }
                    else
                    {
                        currPNode = _mapping[_g1.oppositeNode(nodeOfDepth, fstMatchedE)];
                        edgeItOfDepth = _g2.edge_iterator(currPNode);
                    }
                }
                else
                {
                    currPNode = _g2.oppositeNode(pairOfNodeOfDepth, edgeItOfDepth);
                    subPair(nodeOfDepth,pairOfNodeOfDepth);
                    ++edgeItOfDepth;
                }
                for(; edgeItOfDepth.isValid(); ++edgeItOfDepth)
                {
                    const node currNode = _g2.oppositeNode(currPNode, edgeItOfDepth);
                    if(_conn[currNode]>0 && feas(nodeOfDepth,currNode))
                    {
                        addPair(nodeOfDepth,currNode);
                        break;
                    }
                }
                !edgeItOfDepth.isValid() ? --_depth:++_depth;
            }
            return false;
        }

        public:
        ///Constructor.
        ///\param g1 The graph to be embedded.
        ///\param g2 The graph \e g1 will be embedded into.
        ///\param m The type of the NodeMap storing the mapping.
        ///By default, it is G1::NodeMap<G2::Node>
        ///\param intLabel1 The NodeMap storing the integer node labels of G1.
        ///The labels must be the numbers {0,1,2,..,K-1}, where K is the number of
        ///different labels.
        ///\param intLabel1 The NodeMap storing the integer node labels of G2.
        ///The labels must be the numbers {0,1,2,..,K-1}, where K is the number of
        ///different labels.
        ///\param intLabelEdge1 The EdgeMap storing the integer node labels of G1.
        ///The labels must be the numbers {0,1,2,..,K-1}, where K is the number of
        ///different labels.
        ///\param intLabelEdge2 The EdgeMap storing the integer node labels of G2.
        ///The labels must be the numbers {0,1,2,..,K-1}, where K is the number of
        ///different labels.
        Vf2pp(Graph& g1, Graph& g2, std::vector<node>& m, Vf2ppOrder& order_vf2pp) :
        _g1(g1), _g2(g2), 
        _depth(0), _mapping(m), _order(order_vf2pp._order),
        _conn(g2,0), _currEdgeIts(g1.node_count(),invalidIncEdgeIt), 
        _rNewLabels1(order_vf2pp._rNewLabels1),_rInOutLabels1(order_vf2pp._rInOutLabels1), 
        _intLabels1(g1.get_node_map()) ,_intLabels2(g2.get_node_map()),
        _intLabelsEdge1(g1.get_edge_map()), _intLabelsEdge2(g2.get_edge_map()),
        _labelTmp1(order_vf2pp._labelTmp1),_labelTmp2(order_vf2pp._labelTmp2)
        {
            m = std::vector<node>(g1.node_count(), INVALID);
        }

        bool find() { return extMatch(); }

        ///Destructor
        ~Vf2pp()
        {
            ;
        }
    };
}
