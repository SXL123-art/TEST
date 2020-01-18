//////////////////////////////////////////////////////////////////////
// 模块名：   双向链表模板类
// 最后更新时间： 2010/04/13
//////////////////////////////////////////////////////////////////////

#ifndef TEMPLATEDULLINK_H
#define TEMPLATEDULLINK_H


#if CROSS_WINDOWS
#include <assert.h>
#endif

template <class T>
class CDList;

// 节点
template <class T>
class CDListNode
{
public:
	friend class CDList<T>;

private:
	T data;
	CDListNode<T> * previous;
	CDListNode<T> * next;
};


// 双向链表模板类
template <class T>
class CDList
{
private:
	CDListNode<T> * m_phead;
	CDListNode<T> * m_ptail;
	int m_count;

public:
	// void show(void) const; // 显示节点，便于调试
	CDList();
	~CDList();
	
	// Head/Tail Access Methods
	T & GetHead(void) const;
	T & GetTail(void) const;
	
	// Operations Methods
	void RemoveHead(void);
	void RemoveTail(void);
	int AddHead(const T & NewNode);
	int AddTail(const T & NewNode);
	void RemoveAll(void);
	
	// Iteration Methods
	int GetHeadPosition(void) const;
	int GetTailPosition(void) const;
	T & GetNext(int position) const;
	T & GetPrev(int position) const;
	
	// Retrieval/Modification Methods
	T & GetAt(int position) const;
	void SetAt(int pos, const T & newElement);
	void RemoveAt(int position);
	
	// Insertion Methods
	int InsertBefore(int position,const T & newElement);
	int InsertAfter(int position, const T & newElement);
	
	// Searching Methods
	int Find(const T & searchValue, int startAfter = 1) const;
	
	// Status Methods
	int GetCount(void) const;
	int GetSize(void) const;
	bool IsEmpty(void) const;
};



//////////////////////////////////////////////////////////////////////////
// 实现代码
//////////////////////////////////////////////////////////////////////////

template <class T>
CDList<T>::CDList():m_phead(NULL), m_ptail(NULL), m_count(0)
{
}template <class T>
CDList<T>::~CDList()
{
	RemoveAll();
}


// 获取头结点的值
template <class T>
T & CDList<T>::GetHead(void) const
{
	assert(m_phead != NULL);
	return m_phead->data;
}

// 获取尾结点的值
template <class T>
T & CDList<T>::GetTail(void) const
{
	//assert(m_ptail != NULL);
	//assert(m_ptail != NULL);
	return m_ptail->data;
}


// 删除头结点
template <class T>
void CDList<T>::RemoveHead(void)
{
	if (m_phead != NULL) // 链表不为空
	{
		CDListNode<T> *pRemove;
		pRemove = m_phead;  
		
		if (pRemove->next == NULL) // 只有一个结点
			m_phead = m_ptail = NULL;
		else
		{
			m_phead = m_phead->next;
			m_phead->previous = NULL;
		}  
		
		delete pRemove;
		m_count--;
	}
}

// 删除尾结点
template <class T>
void CDList<T>::RemoveTail(void)
{
	if (m_ptail != NULL) // 链表不为空
	{
		CDListNode<T> *pRemove;
		pRemove = m_ptail;  
		if (pRemove->previous == NULL) // 只有一个结点
			m_phead = m_ptail = NULL;
		else
		{
			m_ptail = m_ptail->previous;
			m_ptail->next = NULL;
		} 
		
		delete pRemove;
		m_count--;
	}
}

// 从链表头部插入结点
template <class T>
int CDList<T>::AddHead(const T & NewNode)
{
	CDListNode<T> *p;
	p = new CDListNode<T>;
	p->data = NewNode;
	p->previous = NULL;
	if (m_phead == NULL) // 链表为空
	{
		p->next = NULL;
		m_ptail = p;
	}
	else
	{
		p->next = m_phead;
		m_phead->previous = p;
	}
	m_phead = p;
	m_count++;return 0;   // 返回插入的节点位置，头节点的位置为0
}

// 从链表尾部插入结点
template <class T>
int CDList<T>::AddTail(const T & NewNode)
{
	CDListNode<T> *p;
	p = new CDListNode<T>;
	p->data = NewNode;
	p->next = NULL;
	if (m_ptail == NULL)// 链表为空
	{
		p->previous = NULL;
		m_phead = p;
	}
	else
	{
		p->previous = m_ptail;
		m_ptail->next = p;
	}
	m_ptail = p;
	m_count++;
	
	return (m_count-1); // 返回插入的节点位置，尾节点的位置为m_count-1
}

// 删除所有结点
template <class T>
void CDList<T>::RemoveAll(void)
{
	if (IsEmpty()){
		return;
	}

	CDListNode<T> *p = m_phead;
	while (m_phead != NULL)
	{
		m_phead = m_phead->next;
		delete p;
		p = m_phead;
		m_count--;
	}
	m_ptail = NULL;
}

// 获取头结点位置
template <class T>
int CDList<T>::GetHeadPosition(void) const
{
	return 0;     // 头节点的位置为0
}

// 获取尾结点位置
template <class T>
int CDList<T>::GetTailPosition(void) const
{
	return (m_count-1);    // 尾节点的位置为m_count-1
}

// 获取某结点的后一个结点值
template <class T>
T & CDList<T>::GetNext(int position) const
{
	return GetAt(position+1);
}

// 获取某结点的前一个结点值
template <class T>
T & CDList<T>::GetPrev(int position) const
{
	return GetAt(position-1);
}

// 获取结点值
template <class T>
T & CDList<T>::GetAt(int position) const
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif
	CDListNode<T> *p = m_phead;
	for (int i=0; i<position; i++) // 定位结点
	{
		p = p->next;
	}

	return p->data;
}

// 修改结点值
template <class T>
void CDList<T>::SetAt(int pos, const T & newElement)
{
#if CROSS_WINDOWS
	assert((pos>=0) && (pos<m_count));
#endif

	CDListNode<T> *p = m_phead;
	for (int i=0; i<pos; i++)
	{
		p = p->next;
	}
	p->data = newElement;
}

// 删除结点
template <class T>
void CDList<T>::RemoveAt(int position)
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	CDListNode<T> *pRemove = m_phead;
	for (int i=0; i<position; i++) // 定位结点
	{
		pRemove = pRemove->next;
	}
	
	if (pRemove->previous == NULL) // 如果是头结点
	{
		RemoveHead();
		return;
	}
	
	if (pRemove->next == NULL) // 如果是尾结点
	{
		RemoveTail();
		return;
	}
	
	CDListNode<T> *p;
	p = pRemove->previous;
	p->next = pRemove->next;
	pRemove->next->previous = p;
	delete pRemove;
	m_count--;
}

// 在某个结点之前插入新结点
template <class T>
int CDList<T>::InsertBefore(int position, const T & newElement)
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	CDListNode<T> *p = m_phead;
	int i;
	for (i=0; i<position; i++) // 定位结点
	{
		p = p->next;
	}
	
	// 在头结点前插入新结点
	if (p->previous == NULL)
		return AddHead(newElement);
	CDListNode<T> *pNewNode;
	pNewNode = new CDListNode<T>;
	pNewNode->data = newElement;
	pNewNode->previous = p->previous;
	pNewNode->next = p;
	p->previous->next = pNewNode;
	p->previous = pNewNode;m_count++;
	return i;
}

// 在某个结点之后插入新结点
template <class T>
int CDList<T>::InsertAfter(int position, const T & newElement )
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	m_count++;CDListNode<T> *p = m_phead;
	int i;
	for (i=0; i<position; i++) // 定位结点
	{
		p = p->next;
	}
	
	// 在尾结点后插入新结点
	if (p->next == NULL)
		return AddTail(newElement);
	CDListNode<T> *pNewNode;
	pNewNode = new CDListNode<T>;
	pNewNode->data = newElement;
	pNewNode->previous = p;
	pNewNode->next = p->next;
	p->next->previous = pNewNode;
	p->next = pNewNode;
	return i+1;
}

// 在链表中查找结点
template <class T>
int CDList<T>::Find(const T & searchValue, int startAfter) const
{
#if CROSS_WINDOWS
	assert((startAfter>=0) && (startAfter<m_count));
#endif

	CDListNode<T> *p = m_phead;
	int i;
	
	// 从startAfter所指的结点向后查找
	for (i=0; i<startAfter; i++) p=p->next;
	{
		// 返回结点在链表中的位置，第一个结点为1
		if (p->data == searchValue)
			return i;
		p = p->next;
		i++;
	}
	
	return -1; // 没有找到
}

// 获取链表结点个数
template <class T>
int CDList<T>::GetCount(void) const
{
	return m_count;
}

// 获取链表结点个数
template <class T>
int CDList<T>::GetSize(void) const
{
	return m_count;
}

// 判断链表是否为空
template <class T>
bool CDList<T>::IsEmpty(void) const
{
	return (m_count == 0) ? true : false;
}

/*
 // 随时可以显示链表结点元素，便于调试
 template <class T>
 void CDList<T>::show(void) const
 {
 CDListNode<T> *p;cout<<"当前链表共有结点："<<GetCount()<<"个";
 cout<<endl<<"结点顺序列表如下：";
 p = m_phead;
 while (p != NULL)
 {
 cout<<" "<< p->data;
 p = p->next;
 }cout<<endl<<"结点逆序列表如下：";
 p = m_ptail;
 while (p != NULL)
 {
 cout<<" "<< p->data;
 p = p->previous;
 }
 cout<<endl<<endl;
 }
 */
#endif