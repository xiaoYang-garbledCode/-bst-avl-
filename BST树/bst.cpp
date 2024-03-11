#include<functional>
#include<iostream>
#include<queue>
#include<stack>
#include<vector>
template<typename T, typename Comp = std::less<T>>
class BSTree
{
public:
	struct Node;
	BSTree(Comp comp = Comp())
		:root_(nullptr),comp_(comp)
	{}
	// 非递归插入操作  ctrl + shift +H 一键替换 less函数的使用
	void n_insert(const T& val)
	{
		// BST树为空
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}

		Node* cur, *parent_;  //用cur去遍历整个BST树, parent_表示指向cur的父结点
		cur = parent_ = root_;
		while (cur)
		{
			parent_ = cur;
			if (cur->data_ == val)
			{
				std::cout << "插入的元素相等，不做插入操作" << std::endl;
				return;
			}else if (comp_(cur->data_, val))  //插入的目标值比当前结点大，向right搜索
			{
				cur = cur->right_;
			}
			else //插入的目标值比当前结点小，向left搜索
			{
				cur = cur->left_;
			}
			// 先不考虑相等的情况
		}
		if (comp_(parent_->data_, val)) // parent_就是当前要插入的结点，现在判断是插入在左还是右孩子
		{
			parent_->right_ = new Node(val);
		}
		else
		{
			parent_->left_ = new Node(val);
		}
		return;
	}
	// 递归插入操作
	void insert(const T& val)
	{
		root_ = insert(root_,val);
	}
	// 非递归删除操作
	void n_remove(const T& val)
	{
		// root为空
		if (root_ == nullptr)
		{
			return;
		}

		Node* cur = root_;
		Node* parent = nullptr;
		// 先找到待删除结点cur
		while (cur)
		{
			if (cur->data_ == val)
			{
				break;  //此时parent为nullptr，因为cur为根结点
			}
			else if (comp_(cur->data_, val)) // val大
			{
				parent = cur;
				cur = cur->right_;
			}
			else {
				parent = cur;
				cur = cur->left_;
			}
		}

		// ！如果没有找到待删除结点
		if (cur == nullptr)
		{
			return;
		}
		// 此时cur指向待删除结点，parent指向cur的父节点
		// 先处理情况三
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			Node* pre = cur->left_;
			while (pre->right_)
			{
				parent = pre;
				pre = pre->right_;
			}
			// 用前驱结点data覆盖待删除结点
			cur->data_ = pre->data_;
			cur = pre; // cur指向前驱结点，此时前驱结点作为待删除结点
		}

		// 此时cur一定不可能有两个孩子
		Node* child = cur->left_;
		if (child == nullptr) 
		{
			child = cur->right_;  //左孩子为空，判断是否有右孩子
		}
		if (parent == nullptr) //此时的parent为空说明，当前的前驱结点是root_的左孩子
		{
			root_->left_ = child;
		}
		else
		{
			// 情况一和二统一处理 因为此时child有三种可能  nullptr  cur->left cur->right
			if (parent->left_ == cur)
			{
				parent->left_ = child;
			}
			else {
				parent->right_ = child;
			}
		}
		delete cur;
		/*
		 先找到待删除结点cur

		 如果cur为空，说明没有找到待删除结点，直接返回

		 先考虑情况三：即cur有两个孩子,找到此时cur的前驱结点，把前驱结点的data值覆盖到cur结点上
		 转换成删除前驱结点即可，前驱结点(左孩子的最右结点)一定不可能有两个孩子。

		 此时cur(此时一定不可能有两个孩子)指向删除结点，parent指向它的父亲结点

		 优化 如何判断cur(只有一个孩子)的孩子是左孩子还是右孩子
		 让child先指向 cur的左孩子， 左孩子为空，那么child就指向右孩子。

		 特殊情况 删除结点为root_,root的parent为空，

		 处理情况一，把parent的对应孩子地址置为空

		 情况二 把parent对应的孩子地址置为child
		*/
	}
	// 递归删除操作
	void remove(const T& val)
	{
		root_ = remove(val);
	}

	// 非递归查询操作
	bool n_query(const T& val)
	{
		Node* p = root_;
		while (p)
		{
			if (p->data_ == val)
			{
				return true;
			}
			else if (comp_(p->data_, val))
			{
				p = p->right_;
			}
			else
			{
				p = p->left_;
			}
		}
		return false;
	}
	// 递归前序遍历
	void preorder()
	{
		preorder(root_);
		std::cout << std::endl;
	}
	// 递归前序遍历
	void inorder()
	{
		inorder(root_);
		std::cout << std::endl;
	}
	// 递归前序遍历
	void postorder()
	{
		postorder(root_);
		std::cout << std::endl;
	}
	// 非递归前序遍历   
	void n_preorder()
	{
		if (root_ == nullptr)
		{
			return;
		}
		Node* p = root_;
		std::stack<Node*> s;
		s.push(p);
		while (!s.empty())
		{
			p = s.top();
			std::cout << p->data_ << " ";
			s.pop(); //当前元素出栈，之后把右左孩子分别入栈。
			if (p->right_ != nullptr) {
				s.push(p->right_);  //右孩子先入栈
			}
			if (p->left_ != nullptr) {
				s.push(p->left_); // 左孩子入栈
			}
		}
	}//当前元素出栈，之后把右左孩子分别入栈。
	// 非递归中序遍历
	void n_inorder()
	{
		if (root_ == nullptr)
		{
			return;
		}
		std::stack<Node*> s;
		Node* p = root_;
		//	要加上p不为空的原因
		//  1.当访问root_结点的右孩子时，此时栈为空但是不能结束while循环，所以
		//  2.刚开始时栈为空
		while (!s.empty() || p != nullptr)
		{
			if (p != nullptr)
			{
				s.push(p);
				p = p->left_;
			}
			else // p 指针为空，此时需要出栈，并访问该元素，之后将p指向它的右孩子
			{
				// 出栈的时候访问
				p = s.top();
				s.pop();
				std::cout << p->data_ << " ";
				p = p->right_;
				// 继续循环(在右孩子继续往左，有孩子则入栈，没有左孩子则出栈(再判断右孩子，有则入栈))
			}
		}
	}
	/*
		前序 VLR  后序 LRV
		先把前序变为 VRL 之后利用栈逆序输出 LRV
	*/
	// 非递归后序遍历
	void n_postorder()
	{
		Node* p = root_;
		std::stack<Node*> s, postStack;
		s.push(p);
		while (!s.empty())
		{
			p = s.top();
			postStack.push(p); // 将前序遍历的结果输入栈中，目的是为了逆序
			s.pop();
			if (p->left_ != nullptr)
			{
				s.push(p->left_);
			}
			if (p->right_ != nullptr)
			{
				s.push(p->right_);
			}
		}
		while (!postStack.empty()) // 输出后序遍历结果
		{
			p = postStack.top();
			std::cout << p->data_ << " ";
			postStack.pop();
		}
	}

	// 非递归层序遍历
	void show()
	{
		std::queue<Node*> que;
		Node* cur = root_;
		if (root_ == nullptr)
		{
			std::cout << "BST为空..." << std::endl;
		}
		que.push(root_);
		while (!que.empty())
		{
			// 打印当前data
			std::cout << que.front()->data_ << " ";

			cur = que.front();
			// 当前结点出队
			que.pop();
			// cur当前结点的左右孩子入队
			if (cur->left_)
			{
				que.push(cur->left_);
			}
			if (cur->right_)
			{
				que.push(cur->right_);
			}
		}
	}

	//递归求二叉树层数
	int high()
	{
		return high(root_);
	}

	//递归求二叉树结点个数
	int number()
	{
		return number(root_);
	}

	// 递归层序遍历
	void levelOrder()
	{
		int h = high();

		for (int i = 0; i < h; i++)
		{
			levelOrder(root_, i);
		}
		std::cout << std::endl;
	}

	// 1求满足区间的元素值[i, j]
	void findValues(std::vector<T>& vec, int i, int j)
	{
		findValues(root_, vec, i, j);
	}
	// 2.判断二叉树是否是BST树的实现，利用BST树的中序遍历是递增的特点
	bool isBSTree()
	{
		return isBSTree(root_, nullptr);
	}
	// 3.判断子树问题  小树有的大树一定有，但大树有的小树不一定有
	bool isChildTree(BSTree<T, Comp>& child)
	{
		// 1.child为空，也是子树
		if (child.root_ == nullptr)
		{
			return true;
		}
		// 2.在大数中找到child的根结点
		Node* fcur = root_; // 用于大树遍历
		while (fcur != nullptr)
		{
			if (fcur->data_ == child.root_->data_)
			{
				break;
			}
			else if (comp_(fcur->data_, child.root_->data_))
			{
				fcur = fcur->right_;
			}
			else
			{
				fcur = fcur->left_;
			}
		}
		if (fcur == nullptr)  // 在大树中没找到小树的根结点，说明不是子树
		{
			return false;
		}
		return isChildTree(fcur, child.root_);
	}
	// 4.BST树的LCA问题，寻找最近公共祖先结点
	int getLCA(int val1, int val2)
	{
		// 始终保持val1<val2
		if (val1 > val2)
		{
			int temp = val2;
			val2 = val1;
			val1 = temp;
		}
		Node* res = getLCA(root_, val1, val2);
		if (res == nullptr)
		{
			throw"no LCA";
		}
		else {
			return res->data_;
		}
	}
	// 5.镜像翻转 
	void mirror01()
	{
		mirror01(root_);
	}
	// 6.镜像对称 
	bool mirror02()
	{
		if (root_ == nullptr)
			return true;
		return mirror02(root_->left_, root_->right_);
	}
	// 7.已知BST树的前序和中序遍历重建BST树
	void rebuild(int pre[], int i,int j, int in[], int m, int n)
	{
		root_ = _rebuild(pre, i, j, in, m, n);
	}
public:
	struct Node
	{
		Node(T data = T()) // 表示零构造
			:left_(nullptr), right_(nullptr), data_(data)
		{}
		Node* left_;
		Node* right_;
		T data_;
	};

	// 递归插入操作
	Node* insert(Node* node, const T& val)
	{
		if (node == nullptr) //找到插入位置
		{
			return new Node(val);
		}
		if (node->data_ == val)
		{
			return node;
		}
		else if (comp_(node->data_, val))
		{
			node->right_ = insert(node->right_, val);
		}
		else {
			node->left_ = insert(node->left_, val);
		}
		return node;
	}
	// 递归前序遍历
	void preorder(Node* node)
	{
		Node*p  = node;
		if (!p)
		{
			return;
		}
		std::cout << p->data_ << " ";
		preorder(p->left_);
		preorder(p->right_);
	}
	// 递归中序遍历
	void inorder(Node* node)
	{
		Node* p = node;
		if (!p)
		{
			return;
		}
		inorder(p->left_);
		std::cout << p->data_ << " ";
		inorder(p->right_);
	}
	// 递归后序遍历
	void postorder(Node* node)
	{
		Node* p = node;
		if (!p)
		{
			return;
		}
		postorder(p->left_);
		postorder(p->right_);
		std::cout << p->data_ << " ";
	}
	// 递归求二叉树层数，求以node为根节点的子树的高度并返回高度值
	int high(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}
		int left_high = high(node->left_);  // L 
		int right_high = high(node->right_); // R

		return (left_high > right_high) ? left_high + 1 : right_high + 1; // V
		//相当于后序遍历，每向上走一层就+1。
	}
	//递归求二叉树结点个数
	int number(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}
		int left_num = number(node->left_);
		int right_num = number(node->right_);

		return left_num + right_num + 1;
	}
	// 递归删除操作
	void remove(Node* node, const T& val)
	{
		if (node == nullptr) // 每找到就返回空， 本来就是空也返回空
			return nullptr;
		if (node->data_ == val) // 找到待删除结点
		{
			// 处理情况3
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				// 找到前驱结点
				Node* pre = node->left_;
				while (pre->right_ != nullptr)
				{
					pre = pre->right_;
				}
				node->data_ = pre->data_;// 把前驱结点的值覆盖待删除结点的值
				// 通过递归直接删除前驱结点
				node->left_ = remove(node->left_, pre->data_);
			}
			// 处理情况1和情况2
			else
			{
				if (node->left_ != nullptr) // 待删除的结点只有左孩子
				{
					// 删除结点以后，把非空的左孩子返回，回溯时更新其父结点的地址域
					Node* left = node->left_;
					delete node;
					return left;
				}
				else if (node->right_ != nullptr)
				{
					// 删除结点以后，把非空的右孩子返回，回溯时更新其父结点的地址域
					Node* right = node->right_;
					delete node;
					return right;
				}
				else // 删除的是没有孩子的结点(叶子结点)
				{
					delete node;
					return nullptr; // 回溯时更新其父亲结点的地址域为nullptr
				}
			}
		}
		// 当前结点的值小于要删除的结点的值
		else if (comp_(node->data_, val))
		{
			node->right_ = remove(node->right_, val);
		}
		// 当前结点的值大于要删除结点的值
		{
			node->left_ = remove(node->left_, val);
		}
		// 把当前结点的值返回给父节点，更新父结点相应的地址域
		return node; 
		/*
			删除结点之后，在回溯时得把删除结点的孩子给删除结点的父结点返回
			1. 如果删除的结点没有孩子，就给父节点返回空
			2. 如果删除的结点是有一个孩子，得把删除结点的孩子给父节点返回
			3. 如果删除的结点有两个孩子，找到待删除结点的前驱结点，把前驱结点值覆盖删除结点的值，之后删除前驱结点
		*/
	}
	// !!!!!!!!!!!!!!!!!!!!======未理解递归层序遍历
	void levelOrder(Node* node, int i)
	{
		if (node == nullptr)
			return;

		if (i == 0)
		{
			std::cout << node->data_ << " ";
			return;
		}
		levelOrder(node->left_, i - 1);
		levelOrder(node->right_, i - 1);
	}

	////node 注意不要写成需要访问完整个BST树
	// 1.求满足区间的元素值[i,j],用vec返回 
	void findValues(Node* node, std::vector<T>& vec, int i, int j)
	{
		// 中序遍历得到的是从小到大的元素顺序
		if (node == nullptr)
		{
			return;
		}
		// 往左子树搜索  L
		if (node->data_ > i) //如果当前结点的值小于左区间，则不进入左子树
		{
			findValues(node->left_, vec, i, j);
		}
		// 访问结点，在区间范围内则放入vec中  V
		if (node->data_ >= i && node->data_ <= j)
		{
			vec.push_back(node->data_);
		}
		// 往右子树搜索   R  // 如果当前结点的值大于右区间，则不进入右子树
		if (node->data_ < j)
		{
			findValues(node->right_, vec, i, j);
		}
	}
	// 2.判断二叉树是否是BST树的实现，利用BST树的中序遍历是递增的特点
	bool isBSTree(Node* node, Node* pre)    // pre作为node的前驱结点,且将pre的初值设为nullptr
	{
		if (node == nullptr)
		{
			return true;
		}
		// 左子树不满足，直接return，不用继续向下
		if (!isBSTree(node->left_, pre))	// L
		{
			return false;
		}

		///////////////////V
		if (pre != nullptr)
		{
			if (comp_(node->data_, pre->data_)) // V 如果前驱结点的值大于node结点，则不是BST树
			{
				return false;
			}
		}
		pre = node;  // 中序遍历此时访问结点，需要更新pre结点。
		///////////////////
	
		// 直接return 右子树的结果
		return isBSTree(node->right_, pre); // R
	}
	// 3.判断子树问题 
	bool isChildTree(Node* fnode, Node* cnode)
	{
		// 分别遍历两树，如果大树没有而小树有则返回fasle
		
		///////////////////////V
		if (fnode == nullptr && cnode== nullptr)  // 都为空
		{
			return true;
		}
		// 走到这说明fnode与cnode两者至少有一个不为空
		if (fnode == nullptr)// fnode为空
		{
			return false; 
		}
		if (cnode == nullptr) // cnode为空
		{
			return true;
		}
		// 两者都不为空
		if (fnode->data_ != cnode->data_)
		{
			return false;
		}
		////////////////////////V

		return (isChildTree(fnode->left_, cnode->left_) //L
			&&isChildTree(fnode->right_, cnode->right_)); //R
	}
	// 4.判断最近祖先结点
	Node* getLCA(Node* node, int val1, int val2)
	{
		// 前序遍历，找到第一个node满足 val1<node<val2
		
		/////////////V
		if (node == nullptr)
		{
			return nullptr;
		}
		//!！ if(val1 < node->data_ < val2) 等价于 (val1 < node->data_) < val2
		if (val1 <= node->data_ && val2 <= node->data_)
		{
			return node;
		}
		/////////////V

		// L
		else if (node->data_ < val1)
		{
			getLCA(node->right_, val1, val2);
		}
		// R
		else if (node->data_ > val2)
		{
			getLCA(node->left_, val1, val2);
		}
	}
	// 5.镜像翻转 
	void mirror01(Node* node)
	{
		// 前序遍历每个结点，并交换它的左右孩子
		if (node == nullptr)
		{
			return;
		}
		//////////// V
		Node* temp = node->left_;
		node->left_ = node->right_;
		node->right_ = temp;

		// L
		mirror01(node->left_);
		// R
		mirror01(node->right_);
	}
	// 6.镜像对称
	bool mirror02(Node* node1, Node* node2)
	{
		// 分别用node1遍历左子树，node2遍历右子树
		//////////////////V
		// 判断node1->left_ = node2->right_   node1->right_ = node2->left_
		// 都为空
		if (node1 == nullptr && node2 == nullptr)
		{
			return true;
		}
		//  都不为空
		else if (node1 != nullptr && node2 != nullptr) 
		{
			//====》相同时应该继续向下递归，不应该返回
			if (node1->data_ != node2->data_)
			{ 
				return false;
			}
		}
		// 两者一个为空一个不为空，不对称
		else
		{
			return false;
		}
		//////////////////V

		return (mirror02(node1->left_, node2->right_) //L
			&& mirror02(node1->right_, node2->left_)); //R
	}
	// 7.已知BST树的前序和中序遍历重建BST树
	Node* _rebuild(int pre[], int i, int j, int in[], int m, int n)
	{
		
	}
	Node* root_;
	Comp comp_;
};

//void test01()
//{
//	BSTree<int> bst;
//	using Node = BSTree<int>::Node;
//	Node* node1 = new Node(40);
//	Node* node2 = new Node(20);
//	Node* node3 = new Node(60);
//	Node* node4 = new Node(30);
//	Node* node5 = new Node(80);
//	bst.root_ = node1;
//	node1->left_ = node2;
//	node1->right_ = node3;
//	node3->left_ = node4;
//	node4->right_ = node5;
//	std::cout<<bst.isBSTree()<<std::endl;
//}

int main()
{
	// 测试mirror2
	BSTree<int> bst;
	using Node = BSTree<int>::Node;
	Node* node1 = new Node(40);
	Node* node2 = new Node(20);
	Node* node3 = new Node(20);
	Node* node4 = new Node(20);
	Node* node5 = new Node(15);
	Node* node6 = new Node(15);
	bst.root_ = node1;
	node1->left_ = node4;
	node1->right_ = node3;
	node4->left_ = node5;
	//node3->right_ = node6;
	std::cout<<bst.mirror02();
	//// 测试 getLAC  镜像翻转
	//BSTree<int> bst;
	//int arr[] = { 58,24,67,0,34,62,69,5,41,64,78 };
	//for (int val : arr)
	//{
	//	bst.n_insert(val);
	//}
	//bst.preorder();
	//std::cout << std::endl;
	//bst.mirror01();
	//bst.preorder();
	//// 测试 isChildTress
	//BSTree<int> bst,bst1;
	//using Node = BSTree<int>::Node;
	//Node* node1 = new Node(67);
	//Node* node2 = new Node(62);
	//Node* node3 = new Node(69);
	//Node* node4 = new Node(30);
	//Node* node5 = new Node(64);
	//bst1.root_ = node1;
	//node1->left_ = node2;
	//node1->right_ = node3;
	////node2->left_ = node4;
	//node4->right_ = node5;
	//int arr[] = { 58,24,67,0,34,62,69,5,41,64,78 };
	//for (int val : arr)
	//{
	//	bst.n_insert(val);
	//}
	//std::cout<<bst.isChildTree(bst1);
	// 测试 n_insert findValues isBSTree
	/*int arr[] = { 58,24,67,0,34,62,69,5,41,64,78,45,70 };
	BSTree<int> bst;
	std::vector<int> vec;
	for (int val : arr)
	{
		bst.n_insert(val);
	}
	bst.findValues(vec, 10, 20);
	for (int val : vec)
	{
		std::cout << val << " ";
	}
	std::cout << bst.isBSTree() << std::endl;
	test01()
	*/
}