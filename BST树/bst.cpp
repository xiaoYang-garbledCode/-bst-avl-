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
	// �ǵݹ�������  ctrl + shift +H һ���滻 less������ʹ��
	void n_insert(const T& val)
	{
		// BST��Ϊ��
		if (root_ == nullptr)
		{
			root_ = new Node(val);
			return;
		}

		Node* cur, *parent_;  //��curȥ��������BST��, parent_��ʾָ��cur�ĸ����
		cur = parent_ = root_;
		while (cur)
		{
			parent_ = cur;
			if (cur->data_ == val)
			{
				std::cout << "�����Ԫ����ȣ������������" << std::endl;
				return;
			}else if (comp_(cur->data_, val))  //�����Ŀ��ֵ�ȵ�ǰ������right����
			{
				cur = cur->right_;
			}
			else //�����Ŀ��ֵ�ȵ�ǰ���С����left����
			{
				cur = cur->left_;
			}
			// �Ȳ�������ȵ����
		}
		if (comp_(parent_->data_, val)) // parent_���ǵ�ǰҪ����Ľ�㣬�����ж��ǲ����������Һ���
		{
			parent_->right_ = new Node(val);
		}
		else
		{
			parent_->left_ = new Node(val);
		}
		return;
	}
	// �ݹ�������
	void insert(const T& val)
	{
		root_ = insert(root_,val);
	}
	// �ǵݹ�ɾ������
	void n_remove(const T& val)
	{
		// rootΪ��
		if (root_ == nullptr)
		{
			return;
		}

		Node* cur = root_;
		Node* parent = nullptr;
		// ���ҵ���ɾ�����cur
		while (cur)
		{
			if (cur->data_ == val)
			{
				break;  //��ʱparentΪnullptr����ΪcurΪ�����
			}
			else if (comp_(cur->data_, val)) // val��
			{
				parent = cur;
				cur = cur->right_;
			}
			else {
				parent = cur;
				cur = cur->left_;
			}
		}

		// �����û���ҵ���ɾ�����
		if (cur == nullptr)
		{
			return;
		}
		// ��ʱcurָ���ɾ����㣬parentָ��cur�ĸ��ڵ�
		// �ȴ��������
		if (cur->left_ != nullptr && cur->right_ != nullptr)
		{
			Node* pre = cur->left_;
			while (pre->right_)
			{
				parent = pre;
				pre = pre->right_;
			}
			// ��ǰ�����data���Ǵ�ɾ�����
			cur->data_ = pre->data_;
			cur = pre; // curָ��ǰ����㣬��ʱǰ�������Ϊ��ɾ�����
		}

		// ��ʱcurһ������������������
		Node* child = cur->left_;
		if (child == nullptr) 
		{
			child = cur->right_;  //����Ϊ�գ��ж��Ƿ����Һ���
		}
		if (parent == nullptr) //��ʱ��parentΪ��˵������ǰ��ǰ�������root_������
		{
			root_->left_ = child;
		}
		else
		{
			// ���һ�Ͷ�ͳһ���� ��Ϊ��ʱchild�����ֿ���  nullptr  cur->left cur->right
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
		 ���ҵ���ɾ�����cur

		 ���curΪ�գ�˵��û���ҵ���ɾ����㣬ֱ�ӷ���

		 �ȿ������������cur����������,�ҵ���ʱcur��ǰ����㣬��ǰ������dataֵ���ǵ�cur�����
		 ת����ɾ��ǰ����㼴�ɣ�ǰ�����(���ӵ����ҽ��)һ�����������������ӡ�

		 ��ʱcur(��ʱһ������������������)ָ��ɾ����㣬parentָ�����ĸ��׽��

		 �Ż� ����ж�cur(ֻ��һ������)�ĺ��������ӻ����Һ���
		 ��child��ָ�� cur�����ӣ� ����Ϊ�գ���ôchild��ָ���Һ��ӡ�

		 ������� ɾ�����Ϊroot_,root��parentΪ�գ�

		 �������һ����parent�Ķ�Ӧ���ӵ�ַ��Ϊ��

		 ����� ��parent��Ӧ�ĺ��ӵ�ַ��Ϊchild
		*/
	}
	// �ݹ�ɾ������
	void remove(const T& val)
	{
		root_ = remove(val);
	}

	// �ǵݹ��ѯ����
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
	// �ݹ�ǰ�����
	void preorder()
	{
		preorder(root_);
		std::cout << std::endl;
	}
	// �ݹ�ǰ�����
	void inorder()
	{
		inorder(root_);
		std::cout << std::endl;
	}
	// �ݹ�ǰ�����
	void postorder()
	{
		postorder(root_);
		std::cout << std::endl;
	}
	// �ǵݹ�ǰ�����   
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
			s.pop(); //��ǰԪ�س�ջ��֮��������ӷֱ���ջ��
			if (p->right_ != nullptr) {
				s.push(p->right_);  //�Һ�������ջ
			}
			if (p->left_ != nullptr) {
				s.push(p->left_); // ������ջ
			}
		}
	}//��ǰԪ�س�ջ��֮��������ӷֱ���ջ��
	// �ǵݹ��������
	void n_inorder()
	{
		if (root_ == nullptr)
		{
			return;
		}
		std::stack<Node*> s;
		Node* p = root_;
		//	Ҫ����p��Ϊ�յ�ԭ��
		//  1.������root_�����Һ���ʱ����ʱջΪ�յ��ǲ��ܽ���whileѭ��������
		//  2.�տ�ʼʱջΪ��
		while (!s.empty() || p != nullptr)
		{
			if (p != nullptr)
			{
				s.push(p);
				p = p->left_;
			}
			else // p ָ��Ϊ�գ���ʱ��Ҫ��ջ�������ʸ�Ԫ�أ�֮��pָ�������Һ���
			{
				// ��ջ��ʱ�����
				p = s.top();
				s.pop();
				std::cout << p->data_ << " ";
				p = p->right_;
				// ����ѭ��(���Һ��Ӽ��������к�������ջ��û���������ջ(���ж��Һ��ӣ�������ջ))
			}
		}
	}
	/*
		ǰ�� VLR  ���� LRV
		�Ȱ�ǰ���Ϊ VRL ֮������ջ������� LRV
	*/
	// �ǵݹ�������
	void n_postorder()
	{
		Node* p = root_;
		std::stack<Node*> s, postStack;
		s.push(p);
		while (!s.empty())
		{
			p = s.top();
			postStack.push(p); // ��ǰ������Ľ������ջ�У�Ŀ����Ϊ������
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
		while (!postStack.empty()) // �������������
		{
			p = postStack.top();
			std::cout << p->data_ << " ";
			postStack.pop();
		}
	}

	// �ǵݹ�������
	void show()
	{
		std::queue<Node*> que;
		Node* cur = root_;
		if (root_ == nullptr)
		{
			std::cout << "BSTΪ��..." << std::endl;
		}
		que.push(root_);
		while (!que.empty())
		{
			// ��ӡ��ǰdata
			std::cout << que.front()->data_ << " ";

			cur = que.front();
			// ��ǰ������
			que.pop();
			// cur��ǰ�������Һ������
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

	//�ݹ������������
	int high()
	{
		return high(root_);
	}

	//�ݹ��������������
	int number()
	{
		return number(root_);
	}

	// �ݹ�������
	void levelOrder()
	{
		int h = high();

		for (int i = 0; i < h; i++)
		{
			levelOrder(root_, i);
		}
		std::cout << std::endl;
	}

	// 1�����������Ԫ��ֵ[i, j]
	void findValues(std::vector<T>& vec, int i, int j)
	{
		findValues(root_, vec, i, j);
	}
	// 2.�ж϶������Ƿ���BST����ʵ�֣�����BST������������ǵ������ص�
	bool isBSTree()
	{
		return isBSTree(root_, nullptr);
	}
	// 3.�ж���������  С���еĴ���һ���У��������е�С����һ����
	bool isChildTree(BSTree<T, Comp>& child)
	{
		// 1.childΪ�գ�Ҳ������
		if (child.root_ == nullptr)
		{
			return true;
		}
		// 2.�ڴ������ҵ�child�ĸ����
		Node* fcur = root_; // ���ڴ�������
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
		if (fcur == nullptr)  // �ڴ�����û�ҵ�С���ĸ���㣬˵����������
		{
			return false;
		}
		return isChildTree(fcur, child.root_);
	}
	// 4.BST����LCA���⣬Ѱ������������Ƚ��
	int getLCA(int val1, int val2)
	{
		// ʼ�ձ���val1<val2
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
	// 5.����ת 
	void mirror01()
	{
		mirror01(root_);
	}
	// 6.����Գ� 
	bool mirror02()
	{
		if (root_ == nullptr)
			return true;
		return mirror02(root_->left_, root_->right_);
	}
	// 7.��֪BST����ǰ�����������ؽ�BST��
	void rebuild(int pre[], int i,int j, int in[], int m, int n)
	{
		root_ = _rebuild(pre, i, j, in, m, n);
	}
public:
	struct Node
	{
		Node(T data = T()) // ��ʾ�㹹��
			:left_(nullptr), right_(nullptr), data_(data)
		{}
		Node* left_;
		Node* right_;
		T data_;
	};

	// �ݹ�������
	Node* insert(Node* node, const T& val)
	{
		if (node == nullptr) //�ҵ�����λ��
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
	// �ݹ�ǰ�����
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
	// �ݹ��������
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
	// �ݹ�������
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
	// �ݹ������������������nodeΪ���ڵ�������ĸ߶Ȳ����ظ߶�ֵ
	int high(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}
		int left_high = high(node->left_);  // L 
		int right_high = high(node->right_); // R

		return (left_high > right_high) ? left_high + 1 : right_high + 1; // V
		//�൱�ں��������ÿ������һ���+1��
	}
	//�ݹ��������������
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
	// �ݹ�ɾ������
	void remove(Node* node, const T& val)
	{
		if (node == nullptr) // ÿ�ҵ��ͷ��ؿգ� �������ǿ�Ҳ���ؿ�
			return nullptr;
		if (node->data_ == val) // �ҵ���ɾ�����
		{
			// �������3
			if (node->left_ != nullptr && node->right_ != nullptr)
			{
				// �ҵ�ǰ�����
				Node* pre = node->left_;
				while (pre->right_ != nullptr)
				{
					pre = pre->right_;
				}
				node->data_ = pre->data_;// ��ǰ������ֵ���Ǵ�ɾ������ֵ
				// ͨ���ݹ�ֱ��ɾ��ǰ�����
				node->left_ = remove(node->left_, pre->data_);
			}
			// �������1�����2
			else
			{
				if (node->left_ != nullptr) // ��ɾ���Ľ��ֻ������
				{
					// ɾ������Ժ󣬰ѷǿյ����ӷ��أ�����ʱ�����丸���ĵ�ַ��
					Node* left = node->left_;
					delete node;
					return left;
				}
				else if (node->right_ != nullptr)
				{
					// ɾ������Ժ󣬰ѷǿյ��Һ��ӷ��أ�����ʱ�����丸���ĵ�ַ��
					Node* right = node->right_;
					delete node;
					return right;
				}
				else // ɾ������û�к��ӵĽ��(Ҷ�ӽ��)
				{
					delete node;
					return nullptr; // ����ʱ�����丸�׽��ĵ�ַ��Ϊnullptr
				}
			}
		}
		// ��ǰ����ֵС��Ҫɾ���Ľ���ֵ
		else if (comp_(node->data_, val))
		{
			node->right_ = remove(node->right_, val);
		}
		// ��ǰ����ֵ����Ҫɾ������ֵ
		{
			node->left_ = remove(node->left_, val);
		}
		// �ѵ�ǰ����ֵ���ظ����ڵ㣬���¸������Ӧ�ĵ�ַ��
		return node; 
		/*
			ɾ�����֮���ڻ���ʱ�ð�ɾ�����ĺ��Ӹ�ɾ�����ĸ���㷵��
			1. ���ɾ���Ľ��û�к��ӣ��͸����ڵ㷵�ؿ�
			2. ���ɾ���Ľ������һ�����ӣ��ð�ɾ�����ĺ��Ӹ����ڵ㷵��
			3. ���ɾ���Ľ�����������ӣ��ҵ���ɾ������ǰ����㣬��ǰ�����ֵ����ɾ������ֵ��֮��ɾ��ǰ�����
		*/
	}
	// !!!!!!!!!!!!!!!!!!!!======δ���ݹ�������
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

	////node ע�ⲻҪд����Ҫ����������BST��
	// 1.�����������Ԫ��ֵ[i,j],��vec���� 
	void findValues(Node* node, std::vector<T>& vec, int i, int j)
	{
		// ��������õ����Ǵ�С�����Ԫ��˳��
		if (node == nullptr)
		{
			return;
		}
		// ������������  L
		if (node->data_ > i) //�����ǰ����ֵС�������䣬�򲻽���������
		{
			findValues(node->left_, vec, i, j);
		}
		// ���ʽ�㣬�����䷶Χ�������vec��  V
		if (node->data_ >= i && node->data_ <= j)
		{
			vec.push_back(node->data_);
		}
		// ������������   R  // �����ǰ����ֵ���������䣬�򲻽���������
		if (node->data_ < j)
		{
			findValues(node->right_, vec, i, j);
		}
	}
	// 2.�ж϶������Ƿ���BST����ʵ�֣�����BST������������ǵ������ص�
	bool isBSTree(Node* node, Node* pre)    // pre��Ϊnode��ǰ�����,�ҽ�pre�ĳ�ֵ��Ϊnullptr
	{
		if (node == nullptr)
		{
			return true;
		}
		// �����������㣬ֱ��return�����ü�������
		if (!isBSTree(node->left_, pre))	// L
		{
			return false;
		}

		///////////////////V
		if (pre != nullptr)
		{
			if (comp_(node->data_, pre->data_)) // V ���ǰ������ֵ����node��㣬����BST��
			{
				return false;
			}
		}
		pre = node;  // ���������ʱ���ʽ�㣬��Ҫ����pre��㡣
		///////////////////
	
		// ֱ��return �������Ľ��
		return isBSTree(node->right_, pre); // R
	}
	// 3.�ж��������� 
	bool isChildTree(Node* fnode, Node* cnode)
	{
		// �ֱ�����������������û�ж�С�����򷵻�fasle
		
		///////////////////////V
		if (fnode == nullptr && cnode== nullptr)  // ��Ϊ��
		{
			return true;
		}
		// �ߵ���˵��fnode��cnode����������һ����Ϊ��
		if (fnode == nullptr)// fnodeΪ��
		{
			return false; 
		}
		if (cnode == nullptr) // cnodeΪ��
		{
			return true;
		}
		// ���߶���Ϊ��
		if (fnode->data_ != cnode->data_)
		{
			return false;
		}
		////////////////////////V

		return (isChildTree(fnode->left_, cnode->left_) //L
			&&isChildTree(fnode->right_, cnode->right_)); //R
	}
	// 4.�ж�������Ƚ��
	Node* getLCA(Node* node, int val1, int val2)
	{
		// ǰ��������ҵ���һ��node���� val1<node<val2
		
		/////////////V
		if (node == nullptr)
		{
			return nullptr;
		}
		//!�� if(val1 < node->data_ < val2) �ȼ��� (val1 < node->data_) < val2
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
	// 5.����ת 
	void mirror01(Node* node)
	{
		// ǰ�����ÿ����㣬�������������Һ���
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
	// 6.����Գ�
	bool mirror02(Node* node1, Node* node2)
	{
		// �ֱ���node1������������node2����������
		//////////////////V
		// �ж�node1->left_ = node2->right_   node1->right_ = node2->left_
		// ��Ϊ��
		if (node1 == nullptr && node2 == nullptr)
		{
			return true;
		}
		//  ����Ϊ��
		else if (node1 != nullptr && node2 != nullptr) 
		{
			//====����ͬʱӦ�ü������µݹ飬��Ӧ�÷���
			if (node1->data_ != node2->data_)
			{ 
				return false;
			}
		}
		// ����һ��Ϊ��һ����Ϊ�գ����Գ�
		else
		{
			return false;
		}
		//////////////////V

		return (mirror02(node1->left_, node2->right_) //L
			&& mirror02(node1->right_, node2->left_)); //R
	}
	// 7.��֪BST����ǰ�����������ؽ�BST��
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
	// ����mirror2
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
	//// ���� getLAC  ����ת
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
	//// ���� isChildTress
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
	// ���� n_insert findValues isBSTree
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