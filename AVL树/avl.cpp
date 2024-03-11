//AVL��   ����ƽ��������
#include<iostream>
template<typename T>
class AVLTree
{
public:
	// ǰ������
	struct Node;

	//AVL�ĳ�ʼ��
	AVLTree() :root_(nullptr) {}

	//AVL���Ĳ������
	void insert(const T& val)
	{
		root_ = insert(root_, val);
	}

	// AVL����ɾ������
	void remove(const T& val)
	{
		root_ = remove(root_, val);
	}
	// ����߶�
	int nodeHeight(Node* node)
	{
		return node == nullptr ? 0 : node->height_;
	}

	// ǰ�����
	void preorder()
	{
		preorder(root_);
	}
private:
	// ǰ������
	struct Node;
	//AVL���Ĳ������
	Node* insert(Node* node, const T& val)
	{
		if (node == nullptr)
		{
			return new Node(val);
		}
		if (node->data_ > val) 
		{
			//==============�ݹ�ʱִ�еĴ���
			node->left_ =  insert(node->left_, val); // ����߲���
			//==============�ݹ����ʱִ�еĴ���
			// ���1 �ڵݹ����ʱ�жϽ���Ƿ�ʧ�⣬ node��������̫�ߣ�nodeʧ��
			if (nodeHeight(node->left_) - nodeHeight(node->right_) > 1)
			{
				if (nodeHeight(node->left_->left_) > nodeHeight(node->left_->right_))
				{
					// ���ں��ӵ�������̫�ߣ���Ҫ��node������������
					node = rightRotate(node);
				}
				else 
{
					// (��ƽ��)�������ӵ�������̫�ߣ���Ҫ�Ժ��ӽ�����������֮���node������������
					node = rightRotate(node);
				}
			}
		}
		else if (node->data_ < val)
		{
			node->right_ = insert(node->right_, val);
			// ���2 �ڵݹ����ʱ�жϽ���Ƿ�ʧ�⣬ node��������̫�ߣ�nodeʧ��
			if (nodeHeight(node->right_) - nodeHeight(node->left_) > 1)
			{
				if (nodeHeight(node->right_) > nodeHeight(node->left_))
				{
					// ���ں��ӵ�������̫�ߣ���Ҫ��node����������
					node = leftRotate(node);
				}
				else
				{
					// (��ƽ��)���ں��ӵ�������̫�ߣ���Ҫ�Ժ��ӽ�����������֮���node������������
					node = rightBalance(node);
				}
			}		
		}
		else {
			// �ҵ���ͬ��㣬���������µݹ��ˣ�ֱ�����ϻ���
		}
		// ���3 ��Ϊ�������������½�㣬�ڵݹ����ʱ�����½��߶ȡ�
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		return node;
	}
	// AVL����ɾ������
	Node* remove(Node* node,const T& val)
	{
		if (node == nullptr) // û���ҵ�Ҫɾ���Ľ��
		{
			return nullptr;
		}
		//�ҵ���ɾ���Ľ��
		if (node->data_ == val)
		{
			// �����
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				// ��ǰ�����̽�㣬Ϊ�˱���ɾ��ǰ�����̽����ɽ��ʧ�⣬���˭��ɾ��˭
				if (nodeHeight(node->left_) >= nodeHeight(node->right_))
				{
					// ɾ��ǰ�����
					Node* pre = node->left_;
					while (pre->right_)
					{
						pre = pre->right_;
					}
					node->data_ = pre->data_;
					node->left_ = remove(node->left_, pre->data_); // ֱ��ɾ��ǰ�����
				}
				// ɾ����̽��
				else
				{
					Node* post = node->right_;
					while (post->left_)
					{
						post = post->left_;
					}
					node->data_ = post->data_;
					node->right_ = remove(node->right_, post->data_);
				}
			}
			// ���һ�������
			else
			{
				// ɾ���Ľ��ֻ��һ������
				if (node->left_ != nullptr)
				{
					Node* left = node->left_;
					delete node;
					return left; // ��ɾ�����ĸ���㷵�����ĺ��ӽ��
				}
				else if (node->right_ != nullptr)
				{
					Node* right = node->right_;
					delete node;
					return right;
				}
				// ��ɾ���Ľ��û�к���
				else
				{
					return nullptr;
				}
			}
		}
		// ɾ������ֵС�ڵ�ǰ�Ľ���ֵ
		else if(node->data_ > val)
		{
			node->left_ = remove(node->left_, val);
			// remove�ݹ���ã�����ִ�����´��룬���������߶�
			// ������ɾ����㣬�����������������
			if (nodeHeight(node->right_) - nodeHeight(node->left_) > 1)
			{
				// �ж������������Һ��Ӹ߶�
				if (nodeHeight(node->right_->left_) <= nodeHeight(node->right_->right_))
				{
					// �Һ��Ӹߣ�˵����Ҫ�����������߶���ȵ�ʱ��Ҳ��ִ������
					node->right_ = leftRotate(node);
				}
				else
				{
					// ���Ӹߣ�˵����Ҫ��ƽ�����
					node->right_ = rightBalance(node);
				}
			}
		}
		// ɾ������ֵ���ڵ�ǰ����ֵ
		else
		{
			node->right_ = remove(node->right_, val);
			// �ݹ����,�����ƽ����Ҫ�������ĸ߶�,ɾ����������,˵���������п��ܸ���
			if (nodeHeight(node->left_) - nodeHeight(node->right_) > 1)
			{
				// �����������Ӹ�
				if (nodeHeight(node->left_->left_) >= nodeHeight(node->left_->right_))
				{
					// ����node��������
					node = rightRotate(node);
				}
				// ���������Һ��Ӹߣ�������ƽ�����
				else
				{
					node = leftBalance(node);
				}
			}
		}

		// ���½��߶�
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		return node; // �ݹ���ݹ����аѵ�ǰ��㷵�ظ����ڵ㡣
	}
	// �������������Ӹ�
	Node* rightRotate(Node* node)
	{
		// �����ת
		Node* child = node->left_;
		node->left_ = child->right_;
		child->right_ = node;

		// �߶ȸ��� node����child���
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		child->height_ = std::max(nodeHeight(child->left_), nodeHeight(child->right_)) + 1;
		// ������ת��������µĸ����
		return child;
	}

	//�����������Һ��Ӹ�
	Node* leftRotate(Node* node)
	{
		Node* child = node->right_;
		node->right_ = child->left_;
		child->left_ = node;

		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		child->height_ = std::max(nodeHeight(child->left_), nodeHeight(child->right_)) + 1;
		return child;
	}

	// ��ƽ��(node�������ӵ���������)����Ҫ������ת(��childΪ��)�������Ӹߵ������
	// �Բ���nodeΪ������-�������������Ѹ���㷵��
	Node* leftBalance(Node *node)
	{
		node->left_ = leftRotate(node->left_); // ����������
		// ��node����������ת
		return rightRotate(node);
	}

	//��ƽ��(node�����Һ�����������)����Ҫ���Һ���Ϊ�����������֮����node���Ϊ������������
	Node* rightBalance(Node* node)
	{
		node->right_ = rightRotate(node->right_);
		return leftRotate(node);
	}

	// ǰ�����
	void preorder(Node* node)
	{
		if (node == nullptr)
			return;
		std::cout << node->data_ << " ";
		preorder(node->left_);
		preorder(node->right_);
	}
private:
	//����AVL���ڵ�����
	struct Node
	{
		Node(T data = T())
			:data_(data)
			,left_(nullptr)
			,right_(nullptr)
			,height_(1)
		{}
		T data_;
		Node* left_;
		Node* right_;
		int height_;//��¼�ڵ�ĸ߶�ֵ
	};
	//ָ����ڵ�
	Node* root_;
};

int main()
{
	AVLTree<int> avl;
	int arr[] = { 1,2,3,4,5,6,7,8,9,10};
	for (int i = 0; i < 10; i++)
	{
		avl.insert(arr[i]);
	}
	avl.preorder();
	avl.remove(4);
	std::cout << std::endl;
	avl.preorder();
	avl.remove(9);
	std::cout<< std::endl;
	avl.preorder();
}