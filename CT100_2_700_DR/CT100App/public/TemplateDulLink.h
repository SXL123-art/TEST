//////////////////////////////////////////////////////////////////////
// ģ������   ˫������ģ����
// ������ʱ�䣺 2010/04/13
//////////////////////////////////////////////////////////////////////

#ifndef TEMPLATEDULLINK_H
#define TEMPLATEDULLINK_H


#if CROSS_WINDOWS
#include <assert.h>
#endif

template <class T>
class CDList;

// �ڵ�
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


// ˫������ģ����
template <class T>
class CDList
{
private:
	CDListNode<T> * m_phead;
	CDListNode<T> * m_ptail;
	int m_count;

public:
	// void show(void) const; // ��ʾ�ڵ㣬���ڵ���
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
// ʵ�ִ���
//////////////////////////////////////////////////////////////////////////

template <class T>
CDList<T>::CDList():m_phead(NULL), m_ptail(NULL), m_count(0)
{
}template <class T>
CDList<T>::~CDList()
{
	RemoveAll();
}


// ��ȡͷ����ֵ
template <class T>
T & CDList<T>::GetHead(void) const
{
	assert(m_phead != NULL);
	return m_phead->data;
}

// ��ȡβ����ֵ
template <class T>
T & CDList<T>::GetTail(void) const
{
	//assert(m_ptail != NULL);
	//assert(m_ptail != NULL);
	return m_ptail->data;
}


// ɾ��ͷ���
template <class T>
void CDList<T>::RemoveHead(void)
{
	if (m_phead != NULL) // ����Ϊ��
	{
		CDListNode<T> *pRemove;
		pRemove = m_phead;  
		
		if (pRemove->next == NULL) // ֻ��һ�����
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

// ɾ��β���
template <class T>
void CDList<T>::RemoveTail(void)
{
	if (m_ptail != NULL) // ����Ϊ��
	{
		CDListNode<T> *pRemove;
		pRemove = m_ptail;  
		if (pRemove->previous == NULL) // ֻ��һ�����
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

// ������ͷ��������
template <class T>
int CDList<T>::AddHead(const T & NewNode)
{
	CDListNode<T> *p;
	p = new CDListNode<T>;
	p->data = NewNode;
	p->previous = NULL;
	if (m_phead == NULL) // ����Ϊ��
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
	m_count++;return 0;   // ���ز���Ľڵ�λ�ã�ͷ�ڵ��λ��Ϊ0
}

// ������β��������
template <class T>
int CDList<T>::AddTail(const T & NewNode)
{
	CDListNode<T> *p;
	p = new CDListNode<T>;
	p->data = NewNode;
	p->next = NULL;
	if (m_ptail == NULL)// ����Ϊ��
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
	
	return (m_count-1); // ���ز���Ľڵ�λ�ã�β�ڵ��λ��Ϊm_count-1
}

// ɾ�����н��
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

// ��ȡͷ���λ��
template <class T>
int CDList<T>::GetHeadPosition(void) const
{
	return 0;     // ͷ�ڵ��λ��Ϊ0
}

// ��ȡβ���λ��
template <class T>
int CDList<T>::GetTailPosition(void) const
{
	return (m_count-1);    // β�ڵ��λ��Ϊm_count-1
}

// ��ȡĳ���ĺ�һ�����ֵ
template <class T>
T & CDList<T>::GetNext(int position) const
{
	return GetAt(position+1);
}

// ��ȡĳ����ǰһ�����ֵ
template <class T>
T & CDList<T>::GetPrev(int position) const
{
	return GetAt(position-1);
}

// ��ȡ���ֵ
template <class T>
T & CDList<T>::GetAt(int position) const
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif
	CDListNode<T> *p = m_phead;
	for (int i=0; i<position; i++) // ��λ���
	{
		p = p->next;
	}

	return p->data;
}

// �޸Ľ��ֵ
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

// ɾ�����
template <class T>
void CDList<T>::RemoveAt(int position)
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	CDListNode<T> *pRemove = m_phead;
	for (int i=0; i<position; i++) // ��λ���
	{
		pRemove = pRemove->next;
	}
	
	if (pRemove->previous == NULL) // �����ͷ���
	{
		RemoveHead();
		return;
	}
	
	if (pRemove->next == NULL) // �����β���
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

// ��ĳ�����֮ǰ�����½��
template <class T>
int CDList<T>::InsertBefore(int position, const T & newElement)
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	CDListNode<T> *p = m_phead;
	int i;
	for (i=0; i<position; i++) // ��λ���
	{
		p = p->next;
	}
	
	// ��ͷ���ǰ�����½��
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

// ��ĳ�����֮������½��
template <class T>
int CDList<T>::InsertAfter(int position, const T & newElement )
{
#if CROSS_WINDOWS
	assert((position>=0) && (position<m_count));
#endif

	m_count++;CDListNode<T> *p = m_phead;
	int i;
	for (i=0; i<position; i++) // ��λ���
	{
		p = p->next;
	}
	
	// ��β��������½��
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

// �������в��ҽ��
template <class T>
int CDList<T>::Find(const T & searchValue, int startAfter) const
{
#if CROSS_WINDOWS
	assert((startAfter>=0) && (startAfter<m_count));
#endif

	CDListNode<T> *p = m_phead;
	int i;
	
	// ��startAfter��ָ�Ľ��������
	for (i=0; i<startAfter; i++) p=p->next;
	{
		// ���ؽ���������е�λ�ã���һ�����Ϊ1
		if (p->data == searchValue)
			return i;
		p = p->next;
		i++;
	}
	
	return -1; // û���ҵ�
}

// ��ȡ���������
template <class T>
int CDList<T>::GetCount(void) const
{
	return m_count;
}

// ��ȡ���������
template <class T>
int CDList<T>::GetSize(void) const
{
	return m_count;
}

// �ж������Ƿ�Ϊ��
template <class T>
bool CDList<T>::IsEmpty(void) const
{
	return (m_count == 0) ? true : false;
}

/*
 // ��ʱ������ʾ������Ԫ�أ����ڵ���
 template <class T>
 void CDList<T>::show(void) const
 {
 CDListNode<T> *p;cout<<"��ǰ�����н�㣺"<<GetCount()<<"��";
 cout<<endl<<"���˳���б����£�";
 p = m_phead;
 while (p != NULL)
 {
 cout<<" "<< p->data;
 p = p->next;
 }cout<<endl<<"��������б����£�";
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