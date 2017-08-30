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
		typedef double ValueType;
		typedef int8_t Depth;
		typedef int32_t Count;

    private:
        NodeIdx       _root {NIL};
        NodeIdx       _nextNodeIdx = 0;

        std::vector<NodeIdx>       _parent;
        std::vector<NodeIdx>       _left;
        std::vector<NodeIdx>       _right;
        std::vector<Depth>      _depth;
        std::vector<Count>       _count;
        std::vector<ValueType>    _values;
        std::vector<Count>       _aggregatedCount;

    public:

        explicit AvlTree();

        AvlTree(const AvlTree&) = delete;
        AvlTree(AvlTree&&) = delete;
        void operator = (const AvlTree&) = delete;
        void operator = (AvlTree&&) = delete;

        //
        // Node comparison
        //

        // O(1)
        inline int compare(const NodeIdx node, const ValueType val) const {
            if(value(node) < val) {
                return 1;
            } else if(value(node) == val) {
                return 0;
            } else {
                return -1;
            }
        }
    
        // O(1)
        inline int compare(const NodeIdx nodeA, const NodeIdx nodeB) const {
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
        inline NodeIdx parentNode(const NodeIdx node) const {
            return _parent[node];
        }
        // O(1)
        inline NodeIdx leftNode(const NodeIdx node) const {
            return _left[node];
        }
        // O(1)
        inline NodeIdx rightNode(const NodeIdx node) const {
            return _right[node];
        }
        // O(1)
        inline Depth depth(const NodeIdx node) const {
            return _depth[node];
        }
        // O(1)
        inline int count(const NodeIdx node) const {
            return _count[node];
        }
        // O(1)
        inline int aggregatedCount(const NodeIdx node) const {
            return _aggregatedCount[node];
        }
        // O(1)
        inline ValueType value(const NodeIdx node) const {
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
            _depth[node] = 1 + std::max(depth(leftNode(node)), depth(rightNode(node)));
            _aggregatedCount[node] = count(node) + aggregatedCount(leftNode(node)) + aggregatedCount(rightNode(node));
        }

        // O(log(n))
        void update(NodeIdx node, ValueType val, Count cnt) {
            _values[node] += cnt * (val - value(node)) / count(node);
            _count[node] += cnt;
            
            for(NodeIdx n = node; n != NIL; n = parentNode(n)) {
               updateAggregates(n);
            }
        }

        // O(log(n))
        void merge(NodeIdx node, ValueType val, Count cnt) {
            assert(value(node) == val);
            _count[node] += cnt;
            
            for(NodeIdx n = node; n != NIL; n = parentNode(n)) {
               updateAggregates(n);
            }
        }

        // O(log(n)) 
        bool add(const ValueType value, const Count cnt);

        // O(log(n))
        NodeIdx find(const ValueType value) const;
        
        // O(log(n))
        NodeIdx floor(const ValueType value) const;

        // O(log(n))
        NodeIdx floorSum(const Count sum) const;

        // O(log(n))
        Count ceilSum(const NodeIdx node) const;

    private:
        // O(1)
        inline Depth balanceFactor(NodeIdx node) const {
            return depth(leftNode(node)) - depth(rightNode(node));
        }

        // (O(log(n)^2)
        void rebalance(const NodeIdx node);

        // O(log(n))
        void rotateLeft(const NodeIdx node);

        // O(log(n))
        // TODO to factor with rotateLeft
        void rotateRight(const NodeIdx node);

		int ExpandNodes();

		int CopyNode(const NodeIdx node, const ValueType val, 
			const Count cnt, const NodeIdx parent);

    public:
        // 
        // For test or debugging purposes
        //

        // Check balance integrity
        bool checkBalance(const NodeIdx node) const;

        bool checkBalance() const;

        // Check aggregates integrity
        bool checkAggregates(const NodeIdx node) const;

        bool checkAggregates() const;

        // Check integrity (order of nodes)
        bool checkIntegrity(const NodeIdx node) const;

        bool checkIntegrity() const;

        // Print as rows
        void print(const NodeIdx node) const;

        void print() const;

};

#endif
