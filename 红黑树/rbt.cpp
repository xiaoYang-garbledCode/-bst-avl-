 // 插入操作总结

/*
1.树空
2.插入位置的父节点为黑色，直接插入红色结点
3.插入位置的父节点为红色
	3.1父节点作为左孩子
		3.1.1 叔叔结点为黑色，或者不存在(结点为空也是黑色)
			3.1.1.1 父亲爷爷孩子不再一条线上
				将当前node设置为父亲结点，并且以node结点进行左旋操作
			3.1.1.2
				走到这的时候，父亲爷爷孩子一定是在一条线上
				将父亲结点置为黑色，爷爷结点置为红色，以爷爷结点为轴进行右旋操作，
				此时操作完毕，爷爷结点一定为黑色，不用向上继续调整
		3.1.2 叔叔结点为红色
			将叔叔和父亲置为黑色，爷爷置为红色。
			将node=爷爷结点，继续向上调整，直到node的父亲为黑色。
	3.2父节点作为右孩子
		同理
*/
template<typename T>
class RBTree
{
public:
	struct  Node;
	RBTree(Node* root = nullptr)
		:root_(root)
	{}
	void insert(const T& val)
	{
		// 1.空树，直接插入黑色结点
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}
		// 寻找插入位置(parent的左孩子或者右孩子)
		Node* cur = root_;
		Node* parent = nullptr;
		while (cur != nullptr)
		{
			parent = cur;
			if (cur->data_ > val)
			{
				cur = cur->left_;
			}
			else if(cur->data_ < val)
			{
				cur = cur->right_;
			}
			else
			{
				return; // 相等结点先不插入
			}
		}
		// 2.非空(插入的结点作为叶子)，检查父亲结点
		// 2.1父亲结点为黑色,直接插入红色结点
		// 插入parent的左孩子
		Node* node = new Node(val, nullptr, nullptr, nullptr, RED);
		if (parent->data_ > val) 
		{
			parent->left_ = node;
		}
		else
		{
			parent->right_ = node;
		}
		// 2.2父亲结点为红色,此时插入红色结点将出现连续的红色,需要进行调整
		if (RED == color(parent))
		{
			fixAferInsert(node);
		}
		return;
	}

	void remove(const T& val)
	{
		if (root_ == nullptr)
		{
			return;
		}
		Node* cur = root_;
		Node* child = nullptr;
		// 先找到待删除的结点
		while (cur != nullptr)
		{
			if (cur->data_ < val)
			{
				cur = cur->right_;
			}
			else if(cur->data_ > val)
			{
				cur = cur->left;
			}
			else
			{
				break;
			}
		}
		// // 如果是空，即没找到val结点，直接返回
		if (cur == nullptr)
		{
			return;
		}
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			// 有两个孩子，寻找前驱结点
			Node* pre = cur->left_;
			while (pre->right_ != nullptr)
			{
				pre = pre->right_;
			}
			cur->data_ = pre->data_;
			cur = pre;
		}
		// 解决情况一和情况二的删除问题，让child指向不为空的孩子
		if (cur->left_ == nullptr)
		{
			child = cur->right_;
		}
		/////////////////如果child不为空 
		if (child != nullptr)// 删除后不用考虑平衡。我们结点有left right parent三个域
		{
			// 让child->parent 指向cur->parent
			child->parent_ = cur->parent_;
			// cur的父结点为空，说明是根结点
			if (cur->parent == nullptr)
			{
				root_ = child; // 让孩子来当根结点
				setColor(child, BLACK);
			}
			// 否则让当前结点的父亲结点(左或右孩子指向child)
			else
			{
				if (cur->parent_->left_ == cur)
				{
					cur->parent_->left_ = child;
				}
				else
				{
					cur->parent_->right_ = child;
				}
			}
			Color c = color(cur);
			delete cur;
			cur = nullptr;
			//如果删除的是黑色结点，则需要进行调整 fixAfterRemove
			if (c == BLACK)
			{
				fixAferRemove(cur);
			}
		}
		/////////////////如果child为空
		else
		{
			// 同样需要判断它的父结点是否为空，为空说明它的根结点
			if (cur->parent == nullptr)
			{
				delete cur;
				root_ = nullptr;
				return;
			}
			// 父结点不为空，删除的cur结点就是叶子结点
			else
			{
				if (color(cur) == BLACK)
				{
					// 如果它是黑色，则需要进行调整 fixAfterRemove
					fixAfterRemove(cur);
				}
				// 并把对应父亲的(左或右孩子置为空)
				if (cur->parent_->left_ == cur)
				{
					cur->parent_->left_ = nullptr;
				}
				else
				{
					cur->parent_->right_ = nullptr;
				}
				delete cur;	
			}
		}
	}

	// 红黑树删除node结点的调整操作
	void fixAfterRemove(Node* node)
	{
		///////// node不是根结点且node是一个黑色结点
		while (node != root_ && color(node) == BLACK)
		{
			////删除的黑色结点在左子树
			if (parent(node)->left_ == node)
			{
				Node* brother = parent(node)->right_;
				// 兄弟结点为红色(说明它们的孩子结点都为黑色) 情况四
				if (color(brother) == RED)
				{
					// 情况四=》将node的父亲结点置为红色，兄弟结点置为黑色，并以父亲结点为轴做左旋
					// 目的是让兄弟结点的黑色孩子成为node结点的兄弟，这样就可以转成情况一或二
					setColor(brother, BLACK);
					setColor(parent(node), RED);
					leftRotate(brother);
				}
				// 兄弟结点为黑色且它的孩子也都为黑色 情况三
				else if(color(brother) == BLACK && color(brother->left_) == BLACK
					&& color(brother->right_) == BLACK)
				{
					// 情况三=》 将兄弟结点置为红色，如果父亲为红色则直接涂成黑色即可
					// 如果父亲结点为黑色，则需要把父亲结点置为红色，继续向上回溯，发现指向的是红色结点，涂成黑色，调整结束
					setColor(brother, RED);
					node = parent(parent(node));
				}
				// 兄弟结点是黑色且它的左孩子为红色 情况二
				else if(color(brother) == BLACK && color(brother->left_) == RED)
				{
					// 情况二=》 将兄弟结点的左孩子涂为黑色，兄弟结点涂为红色，兄弟结点右旋
					// 目的是转成情况一(兄弟结点为黑色，且它的右孩子为红色)
					setColor(brother->left_, BLACK);
					setColor(brother, RED);
					rightRotate(brother);
				}

				// 归结到情况一 
				// 让兄弟结点继承父亲的颜色，并把父亲涂为黑色，兄弟结点的右孩子涂为黑色
				// 以父亲为轴左旋转
				setColor(brother, color(parent(node)));
				setColor(parent(node), BLACK);
				setColor(right(brother), BLACK);
				leftRotate(node(parent));
			}
			else
			{
				////删除的黑色结点在右子树
			}
			
			


			// 如果发现node指向的结点是红色，直接涂成黑色，调整结束
		}
		
	}
	
private:
	// 结点的颜色
	enum Color
	{
		BLACK,
		RED
	};
	// 结点
	struct Node
	{
		Node(T data = T(), Node* parent = nullptr, Node* left = nullptr,
			Node* right = nullptr, Color color = BLACK)
			:left_(left)
			, right_(right)
			, parent_(parent)
			, data_(data)
			, color_(color)
		{}
		Node* left_;
		Node* right_;
		Node* parent_;
		Color color_;
		T data_;
	};
	// 返回结点的颜色
	Color color(Node* node)
	{
		return (node == nullptr) ? BLACK : node->color_;
	}
	// 返回结点的父亲
	Node* parent(Node* node)
	{
		return node->parent_;
	}
	// 返回结点的左孩子
	Node* left(Node* node)
	{
		return node->left_;
	}
	// 返回结点的右孩子
	Node* right(Node* node)
	{
		return node->right_;
	}
	// 设置结点的颜色
	void setColor(Node* node, Color color)
	{
		node->color_ = color;
	}
	// 右旋操作
	void rightRotate(Node* node)
	{
		Node* child = node->left_;
		child->parent_ = node->parent_;
		// node的parent为空，说明node为根结点
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		else
		{
			// node不是根结点，需要判断node是作为左孩子还是右孩子,让其指向child
			if (node->parent_->left_ == node)
			{
				node->parent_->left_ = child;
			}
			else
			{
				node->parent_->right_ = child;
			}
		}
		// 把child右孩子挂到node的左孩子上
		node->left_ = child->right_;
		// 如果child的右孩子不为空，需要让它的parent指向node
		if (child->right_ != nullptr)
		{
			child->right_->parent_ = node;
		}
		// 把node挂到child的右孩子
		child->right_ = node;
		// node的panret指向child
		node->parent_ = child;
	}

	// 左旋操作
	void leftRotate(Node* node)
	{
		Node* child = node->right_;
		child->parent_ = node->parent_;
		// 为空，说明node为根结点
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		// 判断node是左孩子还是右还是，让node的父亲指向child
		else
		{
			if (node->parent_->left_ == node)
			{
				node->parent_->left_ = child;
			}
			else
			{
				node->parent_->right_ = child;
			}
		}
		node->right_ = child->left_;
		// 如果child->left_ 不为空，修改它的parent
		if (child->left_ != nullptr)
		{
			child->left_->parent_ = node;
		}
		child->left_ = node;
		// 修改node的parent
		node->parent_ = child;
	}
    //插入位置的父亲结点为红色，需要做插入调整操作。
	void fixAferInsert(Node* node)
	{
		//0. 如果爷爷被置为红色后，它的父亲也是红色，就需要继续向上调整
		while(RED == color(parent(node)))
		{	
			// 一、父亲作为爷爷的左孩子(叔叔作为右孩子)
			if (left(parent(parent(node))) == parent(node))
			{
				Node* uncle = right(parent(parent(node)));
				//1. 叔叔也为红色(爷爷一定为黑色)，直接将父亲和叔叔置为黑色，爷爷置为红色，需要让爷爷继续向上调整
				if (uncle->color_ == RED)
				{
					setColor(uncle, BLACK);
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					//爷爷继续向上调整
					node = parent(parent(node));
				}
				else
				{
					//2. 叔叔为黑色，判断插入的结点和爷爷不在一侧，需要让父结点左旋来调整到同一侧。
					if (right(parent(node)) == node)
					{
						node = parent(node);
						leftRotate(node);
					}
					// 统一处理：
					//3.交换父亲和爷爷的颜色，并以爷爷为轴进行右旋操作
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					// 爷爷结点右旋
					rightRotate(parent(parent(node)));
					break; // 调整完毕，爷爷结点为黑色，不需要继续向上调整
				}	
			}
			// 二、父亲作为爷爷的右孩子(叔叔作为左孩子)
			else 
			{
				Node* uncle = left(parent(parent(node)));
				//1. 叔叔结点为红色，调整叔叔、父亲为黑色，爷爷为红色，且爷爷结点作为node继续向上调整
				if (color(uncle) == RED)
				{
					setColor(uncle, BLACK);
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				//2. 叔叔结点为黑色，将情况三转为情况二。
				// 情况三：为爷爷,父亲,与插入结点不在一条线上(node在parent的左孩子上)
				if(left(parent(node)) == node)
				{
					// 让node作为父亲结点，且它为轴做右旋操作,
					node = parent(node);
					rightRotate(parent(node));
				}
				//3. 统一处理,爷爷父亲孩子在统一条直线上
				// 把父亲置为黑色，爷爷(起初一定为黑色，因为父亲为红色)置为红色。
				setColor(parent(node), BLACK);
				setColor(parent(parent(node)), RED);
				// 以爷爷为轴做左旋转操作
				leftRotate(parent(parent(node)));
				// 此时调整结束(爷爷结点为黑色)，需要break
				break;
			}	
		}
		//强制将root_置为黑色结点
		setColor(root_, BLACK);
	}

	Node* root_;
};


int main()
{
	RBTree<int> rb;
	for (int i = 1; i <= 4; i++)
	{
		rb.insert(i);
	}
	return 0;
}