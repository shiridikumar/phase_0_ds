#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);

    ifstream fin(pageName, ios::in);
    // cout<<pageName<<endl;
    if(parsedQuery.ismatrix){
        int number;
        vector<int> row(table.maxElementsperblock, 0);
        this->rows.assign(1,row);
        for(int ind=0;ind<table.maxElementsperblock;ind++){
            fin>>number;
            this->rows[0][ind]=number;
        }

    }
    else{
        this->rowCount = table.rowsPerBlockCount[pageIndex];
        int number;

        for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
            {
                fin >> number;
                this->rows[rowCounter][columnCounter] = number;
            }
        }
    }
    fin.close();
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */

vector<int> Page::getRow(int rowIndex,vector<int> sep,vector<int> & result)
{
    logger.log("Page::getRow");
    int init=result.size();
    if (rowIndex >= tableCatalogue.getTable(this->tableName)->rowCount){
        return result;
    }
    if((rowIndex)*columnCount>=(this->pageIndex+1)*tableCatalogue.getTable(this->tableName)->maxElementsperblock){
        return result;
    }
    int ind;
    if(rowIndex==0 || result.size() != 0){
        ind=0;
    }
    else{
        ind=sep[rowIndex-1];
    }

    int count = this->columnCount;

    if(parsedQuery.queryType == PRINT) count = min(20, count);

    count -= init; 

    for(int i=0;i<count;i++){
        
        if(i+ind==this->rows[0].size()){
            break;
        }
        result.push_back(this->rows[0][i+ind]);
    }
    return result;
}

int Page::getRowElement(int rowIndex,int columnindex,vector<int> sep)
{
    return  this->rows[0][columnindex];
}


void Page::getelementsRange(int rowIndex,int start,int end,vector<int>& res)
{
    for(int i=start;i< end;i++){
        res.push_back(this->rows[0][i]);
    }
}
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}
Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}


Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount ,vector<int> sep,int start)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->sep=sep;
    this->start=start;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    if(parsedQuery.ismatrix){
       
        // cout<<"matrix page "<<this->pageName<<endl;
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
           
           if(sep.size()!=0 && columnCounter==sep[this->start]){
                if(columnCounter!=0){
                    fout << endl;
                }
                // cout<<this->start<<" "<<sep.size()<<endl;
                this->start++;
            }
            if(sep.size()>this->start && sep[this->start]==-1){
                // cout<<"end seperator"<<endl;
                break;
            }
            
            fout << this->rows[0][columnCounter]<<" " ;
           
        }


    }
    else{
        for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
            {
                if (columnCounter != 0 && !(parsedQuery.ismatrix))
                    fout << " ";
                fout << this->rows[rowCounter][columnCounter];
            }
            fout << endl;
        }
    }
    fout.close();
    // cout<<"all done"<<endl;

}
