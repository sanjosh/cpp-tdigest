#include "avltree.hpp"
#include <glog/logging.h>

static constexpr size_t kNumNodes = 10;

AvlTree::AvlTree() {
	
	ExpandNodes();
	
    _depth[NIL]     = 0;
    _parent[NIL]    = 0;
    _left[NIL]      = 0;
    _right[NIL]     = 0;
}

AvlTree::NodeIdx AvlTree::first(NodeIdx node) const {
    if(node == NIL) {
        return NIL;
    }

    while(true) {
        const NodeIdx left = leftNode(node);
        if(left == NIL) {
            break;
        }
        node = left;
    }
    return node;
}

AvlTree::NodeIdx AvlTree::last(NodeIdx node) const {
    while(true) {
        const NodeIdx right = rightNode(node);
        if(right == NIL) {
            break;
        }
        node = right;
    }
    return node;
}

AvlTree::NodeIdx AvlTree::nextNode(NodeIdx node) const {
    const NodeIdx right = rightNode(node);
    if(right != NIL) {
        return first(right);
    } else {
        NodeIdx parent = parentNode(node);
        while(parent != NIL && node == rightNode(parent)) {
            node = parent;
            parent = parentNode(parent);
        }
        return parent;
    }
}

AvlTree::NodeIdx AvlTree::prevNode(NodeIdx node) const {
    const NodeIdx left = leftNode(node);
    if(left != NIL) {
        return last(left);
    } else {
        NodeIdx parent = parentNode(node);
        while(parent != NIL && node == leftNode(parent)) {
            node = parent;
            parent = parentNode(parent);
        }
        return parent;
    }
}

AvlTree::NodeIdx AvlTree::ExpandNodes() {
	const size_t new_size = _parent.size() + kNumNodes;
	LOG(INFO) << "resized tree from " << _parent.size() << " to " << new_size;
	_parent.resize(new_size);
	_left.resize(new_size);
	_right.resize(new_size);
	_depth.resize(new_size);
	_count.resize(new_size);
	_values.resize(new_size);
	_aggregatedCount.resize(new_size);
	return 0;
}

AvlTree::NodeIdx 
AvlTree::CopyNode(NodeIdx node, 
		double val, 
		int count, 
		NodeIdx parent) {

	_values[node] = val;
	_count[node] = count;
	_left[node] = NIL;
	_right[node] = NIL;
	_parent[node] = parent;
	return 0;
}

bool AvlTree::add(double value, int count) {
    if(_root == NIL) {
        _root = ++_nextNodeIdx;
        _values[_root] = value;
        _count[_root] = count;
        _left[_root] = NIL;
        _right[_root] = NIL;
        _parent[_root] = NIL;
        // Update depth and aggregates
        updateAggregates(_root);
    } else {
        NodeIdx node = _root;
        NodeIdx parent = NIL;
        int cmp;
        do {
            cmp = compare(node, value);
            if(cmp < 0) {
                parent = node;
                node = leftNode(node);
            } else if (cmp > 0) {
                parent = node;
                node = rightNode(node);
            } else {
                // we merge the node
                merge(node, value, count);
                return false;
            }
        } while(node != NIL);

        node = ++ _nextNodeIdx;
		if (node >= _values.size()) {
			ExpandNodes();
		}
		CopyNode(node, value, count, parent);
        if(cmp < 0) {
            _left[parent] = node;
        } else {
            assert(cmp > 0);
            _right[parent] = node;
        }

        rebalance(node);

        return true;
    }
}

AvlTree::NodeIdx AvlTree::find(double value) const {
    for(NodeIdx node = _root; node != NIL;) {
        const int cmp = compare(node, value);
        if(cmp < 0) {
            node = leftNode(node);
        } else if(cmp > 0) {
            node = rightNode(node);
        } else {
            return node;
        }
    }
    return NIL;
}


AvlTree::NodeIdx AvlTree::floor(double value) const {
    NodeIdx f = NIL;
    for(NodeIdx node = _root; node != NIL; ) {
        const int cmp = compare(node, value);
        if(cmp <= 0) {
            node = leftNode(node);
        } else {
            f = node;
            node = rightNode(node);
        }
    }
    return f;
}

AvlTree::NodeIdx AvlTree::floorSum(long sum) const {
    NodeIdx f = NIL;
    for(NodeIdx node = _root; node != NIL; ) {
        const NodeIdx left = leftNode(node);
        const long leftCount = aggregatedCount(left);
        if(leftCount <= sum) {
            f = node;
            sum -= leftCount + count(node);
            node = rightNode(node);
        } else {
            node = leftNode(node);
        }
    }
    return f;
}

long AvlTree::ceilSum(NodeIdx node) const {
    const NodeIdx left = leftNode(node);
    long sum = aggregatedCount(left);
    NodeIdx n = node;
    for(NodeIdx p = parentNode(node); p != NIL; p = parentNode(n)) {
        if(n == rightNode(p)) {
            const NodeIdx leftP = leftNode(p);
            sum += count(p) + aggregatedCount(leftP);
        }
        n = p;
    }
    return sum;
}

void AvlTree::rebalance(NodeIdx node) {
    for(NodeIdx n = node; n != NIL; ) {
        const NodeIdx p = parentNode(n);

        updateAggregates(n);

        switch(balanceFactor(n)) {
            case -2: {
                         const NodeIdx right = rightNode(n);
                         if(balanceFactor(right) == 1) {
                             rotateRight(right);
                         }
                         rotateLeft(n);
                         break;
                     }
            case 2: {
                        const NodeIdx left = leftNode(n);
                        if(balanceFactor(left) == -1) {
                            rotateLeft(left);
                        }
                        rotateRight(n);
                        break;
                    }
            case -1:
            case  1:
            case  0:
                    break;
            default:
                    // We should throw an error
                    assert(true == false);
        }

        n = p;
    }
}

void AvlTree::rotateLeft(NodeIdx node) {
    const NodeIdx r  = rightNode(node);
    const NodeIdx lr = leftNode(r);

    _right[node] = lr;
    if(lr != NIL) {
        _parent[lr] = node;
    }

    const NodeIdx p = parentNode(node);
    _parent[r] = p;
    if(p == NIL) {
        _root = r;
    } else if(leftNode(p) == node) {
        _left[p] = r;
    } else {
        assert(rightNode(p) == node);
        _right[p] = r;
    }
    _left[r] = node;
    _parent[node] = r;
    updateAggregates(node);
    updateAggregates(parentNode(node));
}

void AvlTree::rotateRight(NodeIdx node) {
    const NodeIdx l = leftNode(node);
    const NodeIdx rl = rightNode(l);

    _left[node] = rl;
    if(rl != NIL) {
        _parent[rl] = node;
    }

    const NodeIdx p = parentNode(node);
    _parent[l] = p;
    if(p == NIL) {
        _root = l;
    } else if(rightNode(p) == node) {
        _right[p] = l;
    } else {
        assert(leftNode(p) == node);
        _left[p] = l;
    }
    _right[l] = node;
    _parent[node] = l;
    updateAggregates(node);
    updateAggregates(parentNode(node));
}
