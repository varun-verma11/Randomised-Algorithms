#include "BloomFilter.hpp"
const unsigned long BloomFilter::m_pocketSize = LONG_BIT;

/***********************************************************/
/******************* PROVIDED FUNCTIONS ********************/
/***********************************************************/

void BloomFilter::init(unsigned long length) {
    m_length = (unsigned long)((2.0*length)/log(2))+1;
    m_pockets = (unsigned long)(ceil(double(m_length)/m_pocketSize));
    m_tickBook.resize(m_pockets);
    unsigned long i;
    for (i=0; i< m_pockets; i++) {
        m_tickBook[i] = 0;
    }
}

unsigned long BloomFilter::hash1(const Key& key) {
    unsigned long hash = 5381;
    unsigned int i=0;
    for (i=0; i< key.length(); i++) {
        hash = ((hash << 5) + hash) + key.c_str()[i]; /* hash * 33 + c */
    }

    double d_hash = (double) hash;

    d_hash *= (0.5*(sqrt(5)-1));
    d_hash -= floor(d_hash);
    d_hash *= (double)m_length;

    return (unsigned long)floor(d_hash);
}

unsigned long BloomFilter::hash2(const Key& key) {
    unsigned long hash = 0;
    unsigned int i=0;
    for (i=0; i< key.length(); i++) {
        hash = key.c_str()[i] + (hash << 6) + (hash << 16) - hash;
    }
    long double d_hash = (long double) hash;

    d_hash *= (0.5*(sqrtl(5)-1));
    d_hash = d_hash/10.0 - floorl(d_hash/10.0);
    d_hash *= (double)m_length;

    return (unsigned long)floorl(d_hash);
}

int BloomFilter::testExist(const Key& key, bool verbose) {
    if (exist(key)) {
        if (verbose) {
            cout<<"Key "<< key<<" is in the set"<<endl;
        }
        return 1;
    } else {
        if (verbose) {
            cout<<"Key "<< key<<" is not in the set"<<endl;
        }
        return 0;
    }
}

void BloomFilter::dump() {
    cout<<m_pockets<<" Pockets: ";
    unsigned long i;
    for (i=0; i< m_pockets; i++) {
        cout<< m_tickBook[i]<<" ";
    }
    cout<<endl;
}

/***********************************************************/
/****************  FUNCTIONS TO BE COMPLETED  ***************/
/***********************************************************/

/////////////////////////////////////////////////////////////
/////////////////////  ADD FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////

void BloomFilter::add(const Key& key) {
    countAdd++;
    ////////////// Write your code below  ////////////////////////
		unsigned long h1 = hash1(key);
		unsigned long h2 = hash2(key);
		
		unsigned int p1 = h1 / m_pocketSize;
		unsigned int p2 = h2 / m_pocketSize;
		
		unsigned long add1 = 1 << (h1 % m_pocketSize);
		unsigned long add2 = 1 << (h2 % m_pocketSize);

		m_tickBook[p1] |= add1;
		m_tickBook[p2] |= add2;		
}


/////////////////////////////////////////////////////////////
/////////////////////  FIND FUNCTIONS ///////////////////////
/////////////////////////////////////////////////////////////


bool BloomFilter::exist(const Key& key) {
    countFind++;
		
		unsigned long h1 = hash1(key);
		unsigned long h2 = hash2(key);	
		
		unsigned int p1 = h1 / m_pocketSize;
		unsigned int p2 = h2 / m_pocketSize;

		unsigned long add1 = 1 << (h1 % m_pocketSize);
		unsigned long add2 = 1 << (h2 % m_pocketSize);

		return (add1 & m_tickBook[p1]) 
			&& (add2 & m_tickBook[p2]); 
}


/////////////////////////////////////////////////////////////
/////////////////////  DEL FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////

void BloomFilter::del(const Key& key) {
    countDelete++;

		unsigned long h1 = hash1(key);
		unsigned long h2 = hash2(key);	
		
		unsigned int p1 = h1 / m_pocketSize;
		unsigned int p2 = h2 / m_pocketSize;

		unsigned long add1 = 1 << (h1 % m_pocketSize);
		unsigned long add2 = 1 << (h2 % m_pocketSize);

		m_tickBook[p1] &= ~add1;
		m_tickBook[p2] &= ~add2;
}


