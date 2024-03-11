 // ��������ܽ�

/*
1.����
2.����λ�õĸ��ڵ�Ϊ��ɫ��ֱ�Ӳ����ɫ���
3.����λ�õĸ��ڵ�Ϊ��ɫ
	3.1���ڵ���Ϊ����
		3.1.1 ������Ϊ��ɫ�����߲�����(���Ϊ��Ҳ�Ǻ�ɫ)
			3.1.1.1 ����үү���Ӳ���һ������
				����ǰnode����Ϊ���׽�㣬������node��������������
			3.1.1.2
				�ߵ����ʱ�򣬸���үү����һ������һ������
				�����׽����Ϊ��ɫ��үү�����Ϊ��ɫ����үү���Ϊ���������������
				��ʱ������ϣ�үү���һ��Ϊ��ɫ���������ϼ�������
		3.1.2 ������Ϊ��ɫ
			������͸�����Ϊ��ɫ��үү��Ϊ��ɫ��
			��node=үү��㣬�������ϵ�����ֱ��node�ĸ���Ϊ��ɫ��
	3.2���ڵ���Ϊ�Һ���
		ͬ��
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
		// 1.������ֱ�Ӳ����ɫ���
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}
		// Ѱ�Ҳ���λ��(parent�����ӻ����Һ���)
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
				return; // ��Ƚ���Ȳ�����
			}
		}
		// 2.�ǿ�(����Ľ����ΪҶ��)����鸸�׽��
		// 2.1���׽��Ϊ��ɫ,ֱ�Ӳ����ɫ���
		// ����parent������
		Node* node = new Node(val, nullptr, nullptr, nullptr, RED);
		if (parent->data_ > val) 
		{
			parent->left_ = node;
		}
		else
		{
			parent->right_ = node;
		}
		// 2.2���׽��Ϊ��ɫ,��ʱ�����ɫ��㽫���������ĺ�ɫ,��Ҫ���е���
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
		// ���ҵ���ɾ���Ľ��
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
		// // ����ǿգ���û�ҵ�val��㣬ֱ�ӷ���
		if (cur == nullptr)
		{
			return;
		}
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			// ���������ӣ�Ѱ��ǰ�����
			Node* pre = cur->left_;
			while (pre->right_ != nullptr)
			{
				pre = pre->right_;
			}
			cur->data_ = pre->data_;
			cur = pre;
		}
		// ������һ���������ɾ�����⣬��childָ��Ϊ�յĺ���
		if (cur->left_ == nullptr)
		{
			child = cur->right_;
		}
		/////////////////���child��Ϊ�� 
		if (child != nullptr)// ɾ�����ÿ���ƽ�⡣���ǽ����left right parent������
		{
			// ��child->parent ָ��cur->parent
			child->parent_ = cur->parent_;
			// cur�ĸ����Ϊ�գ�˵���Ǹ����
			if (cur->parent == nullptr)
			{
				root_ = child; // �ú������������
				setColor(child, BLACK);
			}
			// �����õ�ǰ���ĸ��׽��(����Һ���ָ��child)
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
			//���ɾ�����Ǻ�ɫ��㣬����Ҫ���е��� fixAfterRemove
			if (c == BLACK)
			{
				fixAferRemove(cur);
			}
		}
		/////////////////���childΪ��
		else
		{
			// ͬ����Ҫ�ж����ĸ�����Ƿ�Ϊ�գ�Ϊ��˵�����ĸ����
			if (cur->parent == nullptr)
			{
				delete cur;
				root_ = nullptr;
				return;
			}
			// ����㲻Ϊ�գ�ɾ����cur������Ҷ�ӽ��
			else
			{
				if (color(cur) == BLACK)
				{
					// ������Ǻ�ɫ������Ҫ���е��� fixAfterRemove
					fixAfterRemove(cur);
				}
				// ���Ѷ�Ӧ���׵�(����Һ�����Ϊ��)
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

	// �����ɾ��node���ĵ�������
	void fixAfterRemove(Node* node)
	{
		///////// node���Ǹ������node��һ����ɫ���
		while (node != root_ && color(node) == BLACK)
		{
			////ɾ���ĺ�ɫ�����������
			if (parent(node)->left_ == node)
			{
				Node* brother = parent(node)->right_;
				// �ֵܽ��Ϊ��ɫ(˵�����ǵĺ��ӽ�㶼Ϊ��ɫ) �����
				if (color(brother) == RED)
				{
					// �����=����node�ĸ��׽����Ϊ��ɫ���ֵܽ����Ϊ��ɫ�����Ը��׽��Ϊ��������
					// Ŀ�������ֵܽ��ĺ�ɫ���ӳ�Ϊnode�����ֵܣ������Ϳ���ת�����һ���
					setColor(brother, BLACK);
					setColor(parent(node), RED);
					leftRotate(brother);
				}
				// �ֵܽ��Ϊ��ɫ�����ĺ���Ҳ��Ϊ��ɫ �����
				else if(color(brother) == BLACK && color(brother->left_) == BLACK
					&& color(brother->right_) == BLACK)
				{
					// �����=�� ���ֵܽ����Ϊ��ɫ���������Ϊ��ɫ��ֱ��Ϳ�ɺ�ɫ����
					// ������׽��Ϊ��ɫ������Ҫ�Ѹ��׽����Ϊ��ɫ���������ϻ��ݣ�����ָ����Ǻ�ɫ��㣬Ϳ�ɺ�ɫ����������
					setColor(brother, RED);
					node = parent(parent(node));
				}
				// �ֵܽ���Ǻ�ɫ����������Ϊ��ɫ �����
				else if(color(brother) == BLACK && color(brother->left_) == RED)
				{
					// �����=�� ���ֵܽ�������ͿΪ��ɫ���ֵܽ��ͿΪ��ɫ���ֵܽ������
					// Ŀ����ת�����һ(�ֵܽ��Ϊ��ɫ���������Һ���Ϊ��ɫ)
					setColor(brother->left_, BLACK);
					setColor(brother, RED);
					rightRotate(brother);
				}

				// ��ᵽ���һ 
				// ���ֵܽ��̳и��׵���ɫ�����Ѹ���ͿΪ��ɫ���ֵܽ����Һ���ͿΪ��ɫ
				// �Ը���Ϊ������ת
				setColor(brother, color(parent(node)));
				setColor(parent(node), BLACK);
				setColor(right(brother), BLACK);
				leftRotate(node(parent));
			}
			else
			{
				////ɾ���ĺ�ɫ�����������
			}
			
			


			// �������nodeָ��Ľ���Ǻ�ɫ��ֱ��Ϳ�ɺ�ɫ����������
		}
		
	}
	
private:
	// ������ɫ
	enum Color
	{
		BLACK,
		RED
	};
	// ���
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
	// ���ؽ�����ɫ
	Color color(Node* node)
	{
		return (node == nullptr) ? BLACK : node->color_;
	}
	// ���ؽ��ĸ���
	Node* parent(Node* node)
	{
		return node->parent_;
	}
	// ���ؽ�������
	Node* left(Node* node)
	{
		return node->left_;
	}
	// ���ؽ����Һ���
	Node* right(Node* node)
	{
		return node->right_;
	}
	// ���ý�����ɫ
	void setColor(Node* node, Color color)
	{
		node->color_ = color;
	}
	// ��������
	void rightRotate(Node* node)
	{
		Node* child = node->left_;
		child->parent_ = node->parent_;
		// node��parentΪ�գ�˵��nodeΪ�����
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		else
		{
			// node���Ǹ���㣬��Ҫ�ж�node����Ϊ���ӻ����Һ���,����ָ��child
			if (node->parent_->left_ == node)
			{
				node->parent_->left_ = child;
			}
			else
			{
				node->parent_->right_ = child;
			}
		}
		// ��child�Һ��ӹҵ�node��������
		node->left_ = child->right_;
		// ���child���Һ��Ӳ�Ϊ�գ���Ҫ������parentָ��node
		if (child->right_ != nullptr)
		{
			child->right_->parent_ = node;
		}
		// ��node�ҵ�child���Һ���
		child->right_ = node;
		// node��panretָ��child
		node->parent_ = child;
	}

	// ��������
	void leftRotate(Node* node)
	{
		Node* child = node->right_;
		child->parent_ = node->parent_;
		// Ϊ�գ�˵��nodeΪ�����
		if (node->parent_ == nullptr)
		{
			root_ = child;
		}
		// �ж�node�����ӻ����һ��ǣ���node�ĸ���ָ��child
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
		// ���child->left_ ��Ϊ�գ��޸�����parent
		if (child->left_ != nullptr)
		{
			child->left_->parent_ = node;
		}
		child->left_ = node;
		// �޸�node��parent
		node->parent_ = child;
	}
    //����λ�õĸ��׽��Ϊ��ɫ����Ҫ���������������
	void fixAferInsert(Node* node)
	{
		//0. ���үү����Ϊ��ɫ�����ĸ���Ҳ�Ǻ�ɫ������Ҫ�������ϵ���
		while(RED == color(parent(node)))
		{	
			// һ��������Ϊүү������(������Ϊ�Һ���)
			if (left(parent(parent(node))) == parent(node))
			{
				Node* uncle = right(parent(parent(node)));
				//1. ����ҲΪ��ɫ(үүһ��Ϊ��ɫ)��ֱ�ӽ����׺�������Ϊ��ɫ��үү��Ϊ��ɫ����Ҫ��үү�������ϵ���
				if (uncle->color_ == RED)
				{
					setColor(uncle, BLACK);
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					//үү�������ϵ���
					node = parent(parent(node));
				}
				else
				{
					//2. ����Ϊ��ɫ���жϲ���Ľ���үү����һ�࣬��Ҫ�ø����������������ͬһ�ࡣ
					if (right(parent(node)) == node)
					{
						node = parent(node);
						leftRotate(node);
					}
					// ͳһ����
					//3.�������׺�үү����ɫ������үүΪ�������������
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					// үү�������
					rightRotate(parent(parent(node)));
					break; // ������ϣ�үү���Ϊ��ɫ������Ҫ�������ϵ���
				}	
			}
			// ����������Ϊүү���Һ���(������Ϊ����)
			else 
			{
				Node* uncle = left(parent(parent(node)));
				//1. ������Ϊ��ɫ���������塢����Ϊ��ɫ��үүΪ��ɫ����үү�����Ϊnode�������ϵ���
				if (color(uncle) == RED)
				{
					setColor(uncle, BLACK);
					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				//2. ������Ϊ��ɫ���������תΪ�������
				// �������Ϊүү,����,������㲻��һ������(node��parent��������)
				if(left(parent(node)) == node)
				{
					// ��node��Ϊ���׽�㣬����Ϊ������������,
					node = parent(node);
					rightRotate(parent(node));
				}
				//3. ͳһ����,үү���׺�����ͳһ��ֱ����
				// �Ѹ�����Ϊ��ɫ��үү(���һ��Ϊ��ɫ����Ϊ����Ϊ��ɫ)��Ϊ��ɫ��
				setColor(parent(node), BLACK);
				setColor(parent(parent(node)), RED);
				// ��үүΪ��������ת����
				leftRotate(parent(parent(node)));
				// ��ʱ��������(үү���Ϊ��ɫ)����Ҫbreak
				break;
			}	
		}
		//ǿ�ƽ�root_��Ϊ��ɫ���
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