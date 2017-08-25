#ifndef HEADER_AVLTREE
#define HEADER_AVLTREE

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>


using namespace std;


class AvlTree {

	public:
        static const int NIL = 0;

		typedef int32_t NodeIdx;

    private:
        NodeIdx       _root {NIL};
        NodeIdx       _nextNodeIdx = 0;

        std::vector<NodeIdx>       _parent;
        std::vector<NodeIdx>       _left;
        std::vector<NodeIdx>       _right;
        std::vector<char>      _depth;
        std::vector<int>       _count;
        std::vector<double>    _values;
        std::vector<int>       _aggregatedCount;

    public:

        explicit AvlTree();

        //
        // Node comparison
        //

        // O(1)
        inline int compare(NodeIdx node, double x) const {
            if(value(node) < x) {
                return 1;
            } else if(value(node) == x) {
                return 0;
            } else {
                return -1;
            }
        }
    
        // O(1)
        inline int compare(NodeIdx nodeA, NodeIdx nodeB) const {
            return compare(nodeA, value(nodeB));
        }

        //
        // Tree accessors
        //

        // O(1)
        inline NodeIdx root() const {
            return _root;
        }
        // O(1)
        inline NodeIdx size() const {
            return _nextNodeIdx;
        }

        //
        // Node accessors
        //

        // O(1)
        inline NodeIdx parentNode(NodeIdx node) const {
            return _parent[node];
        }
        // O(1)
        inline NodeIdx leftNode(NodeIdx node) const {
            return _left[node];
        }
        // O(1)
        inline NodeIdx rightNode(NodeIdx node) const {
            return _right[node];
        }
        // O(1)
        inline int depth(NodeIdx node) const {
            return _depth[node];
        }
        // O(1)
        inline int count(NodeIdx node) const {
            return _count[node];
        }
        // O(1)
        inline int aggregatedCount(int node) const {
            return _aggregatedCount[node];
        }
        // O(1)
        inline double value(int node) const {
            return _values[node];
        }

        //
        // Tree accessors
        //

        // O(log(n))
        NodeIdx first(NodeIdx node) const;

        // O(log(n))
        inline NodeIdx first() const {
            return first(_root);
        }

        // O(log(n)) 
        NodeIdx last(NodeIdx node) const;

        // O(log(n))
        NodeIdx nextNode(NodeIdx node) const;

        // O(log(n))
        NodeIdx prevNode(NodeIdx node) const;

        //
        // Mutators
        //

        // O(1)
        inline void updateAggregates(NodeIdx node) {
            // Updating depth
            _depth[node] = 1 + max(depth(leftNode(node)), depth(rightNode(node)));
            _aggregatedCount[node] = count(node) + aggregatedCount(leftNode(node)) + aggregatedCount(rightNode(node));
        }

        // O(log(n))
        void update(NodeIdx node, double x, int w) {
            _values[node] += w * (x - value(node)) / count(node);
            _count[node] += w;
            
            for(int n = node; n != NIL; n = parentNode(n)) {
               updateAggregates(n);
            }
        }

        // O(log(n))
        void merge(NodeIdx node, double x, int w) {
            assert(value(node) == x);
            _count[node] += w;
            
            for(int n = node; n != NIL; n = parentNode(n)) {
               updateAggregates(n);
            }
        }

        // O(log(n)) 
        bool add(double x, int w);

        // O(log(n))
        int find(double x) const;
        
        // O(log(n))
        int floor(double x) const;

        // O(log(n))
        int floorSum(long sum) const;

        // O(log(n))
        long ceilSum(NodeIdx node) const;

    private:
        // O(1)
        inline int balanceFactor(NodeIdx node) const {
            return depth(leftNode(node)) - depth(rightNode(node));
        }

        // (O(log(n)^2)
        void rebalance(NodeIdx node);

        // O(log(n))
        void rotateLeft(NodeIdx node);

        // O(log(n))
        // TODO to factor with rotateLeft
        void rotateRight(NodeIdx node);

		int ExpandNodes();

		int CopyNode(NodeIdx node, double val, int count, NodeIdx parent);

    public:
        // 
        // For test or debugging purposes
        //

        // Check balance integrity
        bool checkBalance(NodeIdx node) const;

        bool checkBalance() const;

        // Check aggregates integrity
        bool checkAggregates(NodeIdx node) const;

        bool checkAggregates() const;

        // Check integrity (order of nodes)
        bool checkIntegrity(NodeIdx node) const;

        bool checkIntegrity() const;

        // Print as rows
        void print(NodeIdx node) const;

        void print() const;

};

#endif
