//AVL树   二叉平衡搜索树
#include<iostream>
template<typename T>
class AVLTree
{
public:
	// 前置声明
	struct Node;

	//AVL的初始化
	AVLTree() :root_(nullptr) {}

	//AVL树的插入操作
	void insert(const T& val)
	{
		root_ = insert(root_, val);
	}

	// AVL树的删除操作
	void remove(const T& val)
	{
		root_ = remove(root_, val);
	}
	// 求结点高度
	int nodeHeight(Node* node)
	{
		return node == nullptr ? 0 : node->height_;
	}

	// 前序遍历
	void preorder()
	{
		preorder(root_);
	}
private:
	// 前置声明
	struct Node;
	//AVL树的插入操作
	Node* insert(Node* node, const T& val)
	{
		if (node == nullptr)
		{
			return new Node(val);
		}
		if (node->data_ > val) 
		{
			//==============递归时执行的代码
			node->left_ =  insert(node->left_, val); // 向左边插入
			//==============递归回溯时执行的代码
			// 添加1 在递归回溯时判断结点是否失衡， node的左子树太高，node失衡
			if (nodeHeight(node->left_) - nodeHeight(node->right_) > 1)
			{
				if (nodeHeight(node->left_->left_) > nodeHeight(node->left_->right_))
				{
					// 由于孩子的左子树太高，需要对node结点进行右旋、
					node = rightRotate(node);
				}
				else 
{
					// (左平衡)由于左孩子的右子树太高，需要以孩子结点进行左旋，之后对node结点进行右旋。
					node = rightRotate(node);
				}
			}
		}
		else if (node->data_ < val)
		{
			node->right_ = insert(node->right_, val);
			// 添加2 在递归回溯时判断结点是否失衡， node的右子树太高，node失衡
			if (nodeHeight(node->right_) - nodeHeight(node->left_) > 1)
			{
				if (nodeHeight(node->right_) > nodeHeight(node->left_))
				{
					// 由于孩子的右子树太高，需要对node结点进行左旋
					node = leftRotate(node);
				}
				else
				{
					// (右平衡)由于孩子的左子树太高，需要以孩子结点进行右旋，之后对node结点进行左旋。
					node = rightBalance(node);
				}
			}		
		}
		else {
			// 找到相同结点，不用再往下递归了，直接向上回溯
		}
		// 添加3 因为子树中增加了新结点，在递归回溯时检查更新结点高度。
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		return node;
	}
	// AVL树的删除操作
	Node* remove(Node* node,const T& val)
	{
		if (node == nullptr) // 没有找到要删除的结点
		{
			return nullptr;
		}
		//找到待删除的结点
		if (node->data_ == val)
		{
			// 情况三
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				// 找前驱或后继结点，为了避免删除前驱或后继结点造成结点失衡，因此谁高删除谁
				if (nodeHeight(node->left_) >= nodeHeight(node->right_))
				{
					// 删除前驱结点
					Node* pre = node->left_;
					while (pre->right_)
					{
						pre = pre->right_;
					}
					node->data_ = pre->data_;
					node->left_ = remove(node->left_, pre->data_); // 直接删除前驱结点
				}
				// 删除后继结点
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
			// 情况一和情况二
			else
			{
				// 删除的结点只有一个孩子
				if (node->left_ != nullptr)
				{
					Node* left = node->left_;
					delete node;
					return left; // 向删除结点的父结点返回它的孩子结点
				}
				else if (node->right_ != nullptr)
				{
					Node* right = node->right_;
					delete node;
					return right;
				}
				// 待删除的结点没有孩子
				else
				{
					return nullptr;
				}
			}
		}
		// 删除结点的值小于当前的结点的值
		else if(node->data_ > val)
		{
			node->left_ = remove(node->left_, val);
			// remove递归调用，回溯执行以下代码，调整子树高度
			// 左子树删除结点，可能造成右子树更高
			if (nodeHeight(node->right_) - nodeHeight(node->left_) > 1)
			{
				// 判断右子树的左右孩子高度
				if (nodeHeight(node->right_->left_) <= nodeHeight(node->right_->right_))
				{
					// 右孩子高，说明需要左旋操作。高度相等的时候也是执行左旋
					node->right_ = leftRotate(node);
				}
				else
				{
					// 左孩子高，说明需要右平衡操作
					node->right_ = rightBalance(node);
				}
			}
		}
		// 删除结点的值大于当前结点的值
		else
		{
			node->right_ = remove(node->right_, val);
			// 递归回溯,如果不平衡需要调整树的高度,删除的右子树,说明左子树有可能更高
			if (nodeHeight(node->left_) - nodeHeight(node->right_) > 1)
			{
				// 左子树的左孩子高
				if (nodeHeight(node->left_->left_) >= nodeHeight(node->left_->right_))
				{
					// 进行node右旋操作
					node = rightRotate(node);
				}
				// 左子树的右孩子高，进行左平衡操作
				else
				{
					node = leftBalance(node);
				}
			}
		}

		// 更新结点高度
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		return node; // 递归回溯过程中把当前结点返回给父节点。
	}
	// 右旋操作，左孩子高
	Node* rightRotate(Node* node)
	{
		// 结点旋转
		Node* child = node->left_;
		node->left_ = child->right_;
		child->right_ = node;

		// 高度更新 node结点和child结点
		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		child->height_ = std::max(nodeHeight(child->left_), nodeHeight(child->right_)) + 1;
		// 返回旋转后的子树新的根结点
		return child;
	}

	//左旋操作，右孩子高
	Node* leftRotate(Node* node)
	{
		Node* child = node->right_;
		node->right_ = child->left_;
		child->left_ = node;

		node->height_ = std::max(nodeHeight(node->left_), nodeHeight(node->right_)) + 1;
		child->height_ = std::max(nodeHeight(child->left_), nodeHeight(child->right_)) + 1;
		return child;
	}

	// 左平衡(node结点的左孩子的右子树高)，需要先左旋转(以child为轴)化成左孩子高的情况。
	// 以参数node为轴做左-右旋操作，并把根结点返回
	Node* leftBalance(Node *node)
	{
		node->left_ = leftRotate(node->left_); // 孩子先左旋
		// 让node结点进行右旋转
		return rightRotate(node);
	}

	//右平衡(node结点的右孩子左子树高)，需要以右孩子为轴进行右旋，之后以node结点为轴做左旋操作
	Node* rightBalance(Node* node)
	{
		node->right_ = rightRotate(node->right_);
		return leftRotate(node);
	}

	// 前序遍历
	void preorder(Node* node)
	{
		if (node == nullptr)
			return;
		std::cout << node->data_ << " ";
		preorder(node->left_);
		preorder(node->right_);
	}
private:
	//定义AVL树节点类型
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
		int height_;//记录节点的高度值
	};
	//指向根节点
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