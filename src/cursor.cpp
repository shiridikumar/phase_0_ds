#include "global.h"

Cursor::Cursor(string tableName, int pageIndex)
{
    // cout<<"cursor function"<<endl;
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(tableName, pageIndex);
    this->pagePointer = 0;
    this->tableName = tableName;
    this->pageIndex = pageIndex;
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to).
 *
 * @return vector<int> 
 */
vector<int> Cursor::getNext()
{
    if(parsedQuery.ismatrix){
        logger.log("Cursor::geNext");
        vector<int> sep=tableCatalogue.getTable(this->tableName)->sep;
        vector<int> result;
        this->page.getRow(this->pagePointer,sep,result);
        while(result.size()<min(20,(int)tableCatalogue.getTable(this->tableName)->columnCount) && this->pagePointer<tableCatalogue.getTable(this->tableName)->rowCount){
          
            int temp=this->pagePointer;
            tableCatalogue.getTable(this->tableName)->getNextPage(this);
            if(!(pagePointer)){
                this->pagePointer=temp;
                this->page.getRow(this->pagePointer,sep,result);
            }
            else{
                break;
            }
            if(this->pageIndex==tableCatalogue.getTable(this->tableName)->blockCount-1){
                break;
            }
        }
        this->pagePointer++;
        return result;
        
    }
    else{
        logger.log("Cursor::geNext");
        vector<int> result = this->page.getRow(this->pagePointer);
        this->pagePointer++;
        if(result.empty()){
            tableCatalogue.getTable(this->tableName)->getNextPage(this);
            if(!this->pagePointer){
                result = this->page.getRow(this->pagePointer);
                this->pagePointer++;
            }
            
        }
        return result;
    }
    
}

vector<int> Cursor::getnextline()
{
          
    
}


/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex 
 */
void Cursor::nextPage(int pageIndex)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->tableName, pageIndex);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}