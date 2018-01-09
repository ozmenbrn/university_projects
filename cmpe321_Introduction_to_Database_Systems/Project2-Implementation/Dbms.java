/*Student
 * name: Barýn Özmen
 * id: 2012400045
 * status: Done
 */

import java.io.*;
import java.util.*;


public class Dbms {

	public static void main(String[] args) throws IOException, FileNotFoundException {
		Scanner console = new Scanner(System.in);			//makes able to read input from the user
		File dbms = new File("dbms.dat");					//helps to create a physical folder names dbms.dat 
		int numPage = 0, numType = 0, numRec = 0;			
		boolean isFull = false;
		String temp ="";
		
		RandomAccessFile fileWriter= new RandomAccessFile(dbms, "rw");
		
		if (dbms.createNewFile()){							
			System.out.println("Welcome to DBMS!");			//create the file
		}else{
			System.out.println("Welcome to DBMS!");
		}
		
		/**
		 * If the database file is not exist and created for the first time, than do the following to file
		 * It is also called SystemCatalog file.
		 */
		String check = fileWriter.readLine();
		if(check == null){
			fileWriter.writeBytes("Number Of Types: "+numType+ "      \n");
			fileWriter.readLine();
			fileWriter.writeBytes("Number of Pages: "+numPage+ "      \n");
			fileWriter.readLine();
			fileWriter.writeBytes("Number of Records: "+numRec+ "    \n");
			fileWriter.readLine();
			fileWriter.writeBytes("Storage Info: 1MB/"+ fileWriter.length()+ "      \n");
			fileWriter.readLine();
			fileWriter.writeBytes("TypeNames \n");
			fileWriter.readLine();
			for(int i=0; i<10; i++){
				fileWriter.writeBytes("          \n");
				fileWriter.readLine();
			}
		}
		
		
		boolean isFinished = false;					//to get info from the user and decide to terminate the program.
		
		
		while(!isFinished){
			System.out.println("\n\nSelect Your Operation\n1: DDL Operations\n2: DML Operations\n3: Quit");
			fileWriter.close();
			fileWriter = new RandomAccessFile(dbms, "rw");
			for(int f = 0; f<5; f++){    
				fileWriter.readLine();
			}
			numType = 0;
			for(int f = 0; f<10; f++){
				temp = fileWriter.readLine();
				if(temp.substring(0, 5).equals("     ")){
					continue;
				}
				else{
					numType++;
				}
			}
			if(numType==10){
				isFull= true;
			}
			else{
				isFull= false;
			}
			
			//The operation will be selected after that part.
			int operationNum = console.nextInt();
			if(operationNum == 1){
				System.out.println("Select a DDL Operation\n1: Create a Type\n2: Delete a type\n3: List all types");
				operationNum = console.nextInt();
				if(operationNum == 1  && !isFull){
					createType(fileWriter, console, dbms);
				}
				else if(operationNum == 1 && isFull){
					System.out.println("Storage is Full!!");
				}
				else if(operationNum == 2){
					deleteType(fileWriter, console, dbms);
				}
				else if(operationNum == 3){
					listAllTypes(fileWriter, console, dbms);
				}
				else{
					System.out.println("Assume that the user always inputs valid data!");				
				}
			}
			else if(operationNum == 2){
				System.out.println("Select a DML Operation\n1: Create a Record\n2: Delete a Record\n"
						+ "3: Search for a Record(by Primary Key)\n4: List all records of a type");
				operationNum = console.nextInt();
				if(operationNum == 1){
					createRecord(fileWriter, console, dbms);
				}
				else if(operationNum == 2){
					deleteRecord(fileWriter, console, dbms);
				}
				else if(operationNum == 3){
					searchRecord(fileWriter, console, dbms);
				}
				else if(operationNum == 4){
					listRecords(fileWriter, console, dbms);
				}
				else{
					System.out.println("Assume that the user always inputs valid data!");				
				}
			}
			else if(operationNum == 3){
				isFinished = true;
			}
			else{
				System.out.println("Assume that the user always inputs valid data!");
			}
			
		}
		
		/**
		 * Files last update part. 
		 */
		int count = 0;
		numPage = numType*75;
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		fileWriter.writeBytes("Number Of Types: "+numType);
		fileWriter.readLine();
		fileWriter.writeBytes("Number of Pages: "+numPage);
		fileWriter.readLine();
		fileWriter.writeBytes("Number of Records:      ");
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		String tempReader = fileWriter.readLine();
		while(tempReader!=null){
			if(tempReader.substring(0, 9).equals("newRecord")){
				count++;
			}
			tempReader = fileWriter.readLine();
		}
		numRec = count;
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		fileWriter.readLine();
		fileWriter.readLine();
		
		fileWriter.writeBytes("Number of Records: "+numRec);
		fileWriter.readLine();
		
		fileWriter.writeBytes("Storage Info: 1MB/"+ fileWriter.length());
		
		fileWriter.close();
	}
	/**
	 * Create Type
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void createType(RandomAccessFile fileWriter, Scanner console, File dbms) throws IOException {
		// TODO Auto-generated method stub
		Random rand = new Random();
		boolean isExist = false;
		System.out.println("Enter a name for type(also called as primary key)");
		String typeName = console.next();
		String current = "", prev = "", rel;
		int typeLength;
			
		if(typeName.length()>10){
			typeName = typeName.substring(0, 10);
		}
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i=0; i<5; i++){
			fileWriter.readLine();
		}
		
		typeLength = typeName.length();
		for(int j=0; j<10; j++){
			rel = fileWriter.readLine();
			if(rel.substring(0, typeLength).equals(typeName)){
				isExist = true;
				break;
			}
		}
		if(isExist == true){
			System.out.println("Type is already exist!");
		}
		else{
			fileWriter.close();
			fileWriter = new RandomAccessFile(dbms, "rw");
			for(int i=0; i<4; i++){
				fileWriter.readLine();
			}
			for(int j=0; j<11; j++){
				current = fileWriter.readLine();
				if(current.substring(0, 5).equals("     ")){
					fileWriter.close();
					fileWriter = new RandomAccessFile(dbms, "rw");
					current = fileWriter.readLine();
					while(!current.equals(prev)){
						current = fileWriter.readLine();
					}
					fileWriter.writeBytes(typeName);
					break;
				}
				prev = current;
			}
			
			current = fileWriter.readLine();
			while(current != null ){     
				prev=current;
				current = fileWriter.readLine();
				if(current != null){
					if(current.substring(0, 10).equals("typeErased")){
						break;
					}
				}
			}
			
			if(current==null){
				typeLength = typeName.length();
				
				int randomNum = 100 + rand.nextInt(899);
				fileWriter.writeBytes("newType"+randomNum+"\n");
				fileWriter.readLine();
				fileWriter.writeBytes(typeName);
				for(int k=10; k>typeLength; k--){
					fileWriter.writeBytes(" ");
				}
				fileWriter.writeBytes("\n");
				fileWriter.readLine();
				fileWriter.writeBytes("Available \n");
				fileWriter.readLine();
				fileWriter.writeBytes("* NextType\n");
				fileWriter.readLine();
				createPages(fileWriter, console, dbms);
				fileWriter.writeBytes("endType"+randomNum+"\n");
			}
			else{
				fileWriter.close();
				fileWriter = new RandomAccessFile(dbms, "rw");
				current = fileWriter.readLine();
				while(!current.equals(prev)){
					current = fileWriter.readLine();
				}
				fileWriter.writeBytes(typeName);
				for(int k=10; k>typeLength; k--){
					fileWriter.writeBytes(" ");
				}
				fileWriter.readLine();
				fileWriter.writeBytes("Available ");
				fileWriter.readLine();
				fileWriter.writeBytes("* NextType");
				fileWriter.readLine();
				updatePages(fileWriter, console, dbms);		
			}
			System.out.println("Type "+typeName+ " is created");

		}
		
	}
	
	/**
	 * update pages
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void updatePages(RandomAccessFile fileWriter, Scanner console, File dbms) throws IOException {
		Random rand = new Random();
		
		for(int j=0; j<75; j++){
			fileWriter.writeBytes("newPage   ");
			fileWriter.readLine();
			fileWriter.writeBytes("PageID:"+(100+rand.nextInt(899)));
			fileWriter.readLine();
			fileWriter.writeBytes("Available ");
			fileWriter.readLine();
			fileWriter.writeBytes("* NextPage");
			fileWriter.readLine();
			for(int i=0; i<120; i++){
				fileWriter.writeBytes("          ");
				fileWriter.readLine();
			}
		}
		
	}

	/**
	 * create pages
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void createPages(RandomAccessFile fileWriter, Scanner console, File dbms) throws IOException {
		// TODO Auto-generated method stub
		Random rand = new Random();
		
		for(int j=0; j<75; j++){
			fileWriter.writeBytes("newPage   \n");
			fileWriter.readLine();
			fileWriter.writeBytes("PageID:"+(100+ rand.nextInt(899))+"\n");
			fileWriter.readLine();
			fileWriter.writeBytes("Available \n");
			fileWriter.readLine();
			fileWriter.writeBytes("* NextPage\n");
			fileWriter.readLine();
			for(int i=0; i<120; i++){
				fileWriter.writeBytes("          \n");
				fileWriter.readLine();
			}
		}
	}

	/**
	 * deletes a type
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void deleteType(RandomAccessFile fileWriter, Scanner console, File dbms) throws IOException  {
		// TODO Auto-generated method stub
		boolean isExist = false;
		String current = "", prev = "";
		System.out.println("Enter name(primary key) of the type");
		String typeName = console.next();
		int typeLength = typeName.length();
		Random rand = new Random();
		String tempTypeName = typeName;
		for(int t=10; t>typeLength; t--){
			tempTypeName+=" ";
		}
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i=0; i<15; i++){
			prev = current;
			current = fileWriter.readLine();
			if(current.substring(0, 10).equals(tempTypeName)){
				isExist = true;
				break;
			}
		}
		
		if(!isExist){
			System.out.println("Type doesn't exist");
		}
		else{
			fileWriter.close();
			fileWriter = new RandomAccessFile(dbms, "rw");
			
			current = fileWriter.readLine();
			for(int l = 0; l<15; l++){
				if(current.equals(prev)){
					fileWriter.writeBytes("          ");
				}
				current = fileWriter.readLine();
			}
			
		
			while(!current.substring(0, 10).equals(tempTypeName)){
				prev = current;
				current = fileWriter.readLine();
			}
			fileWriter.close();
			fileWriter = new RandomAccessFile(dbms, "rw");
			current = fileWriter.readLine();
			while(!current.equals(prev)){
				current = fileWriter.readLine();
			}
			
			fileWriter.writeBytes("typeErased");
			fileWriter.readLine();
			fileWriter.writeBytes("          ");
			fileWriter.readLine();
			fileWriter.writeBytes("          ");
			fileWriter.readLine();
			
			for(int j=0; j<75; j++){
				fileWriter.writeBytes("          ");
				fileWriter.readLine();
				fileWriter.writeBytes("          ");
				fileWriter.readLine();
				fileWriter.writeBytes("          ");
				fileWriter.readLine();
				fileWriter.writeBytes("          ");
				fileWriter.readLine();
				for(int i=0; i<120; i++){
					fileWriter.writeBytes("          ");
					fileWriter.readLine();
				}
			}
			System.out.println("Type "+typeName+ " is deleted from the DataBase");
			
		}
		
	}
	
	/**
	 * Listing all the types
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void listAllTypes(RandomAccessFile fileWriter, Scanner console, File dbms)throws IOException  {
		// TODO Auto-generated method stub
		String[] current = new String[10];
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i =0; i<5; i++){
			fileWriter.readLine();
		}

		for(int j=0; j<10; j++){
			current[j] = fileWriter.readLine();
		}
		
		System.out.println("List of all types:");
		System.out.print("[");
		for(int j=0; j<10; j++){
			if(current[j].substring(0, 5).equals("     ")){
				continue;
			}
			else{
				System.out.print(" " + current[j] + " ");
			}
		}
		System.out.println("]");
		
	}
	
	/**
	 * Creating records
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void createRecord(RandomAccessFile fileWriter, Scanner console, File dbms)throws IOException  {
		// TODO Auto-generated method stub
		System.out.println("Enter the type you want to create a record");
		boolean isExist = false;
		String current = "", prev = "";
		String typeName = console.next();
		int typeLength = typeName.length();
		Random rand = new Random();
		if(typeName.length() > 10){
			typeName = typeName.substring(0, 10);
		}
		String tempTypeName = typeName;
		for(int t=10; t>typeLength; t--){
			tempTypeName+=" ";
		}
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i=0; i<15; i++){
			prev = current;
			current = fileWriter.readLine();
			if(current.substring(0, 10).equals(tempTypeName)){
				isExist = true;
				break;
			}
		}
		
		if(!isExist){
			System.out.println("Type doesn't exist");
		}
		else{
			int recordNum = 0;
			long position = 0;
			boolean isFull= false;
			String answer ="";
			current = fileWriter.readLine();
			while(!current.substring(0, 10).equals(tempTypeName)){
				current = fileWriter.readLine();
			}
			while(!current.substring(0, 7).equals("       ")){
				position = fileWriter.getFilePointer();
				current = fileWriter.readLine();
				if(current.substring(0, 7).equals("endType")){
					isFull = true;
				}
			}		
			if(!isFull){
				fileWriter.close();
				fileWriter = new RandomAccessFile(dbms, "rw");
				fileWriter.seek(position);
				fileWriter.writeBytes("newRecord ");
				fileWriter.readLine();
				
				System.out.println("How many field you want to use for record(min-1, max-9)");
				recordNum = console.nextInt();
				System.out.println("Enter the Primary Key of the Record");
				answer = console.next();
				if(answer.length()>10){
					answer = answer.substring(0, 10);
				}
				fileWriter.writeBytes(answer);
				fileWriter.readLine();
				fileWriter.writeBytes("RecID:"+(1000 + rand.nextInt(8999)));
				fileWriter.readLine();
				fileWriter.writeBytes(answer);
				fileWriter.readLine();
				for(int b=0; b<recordNum-1; b++){
					System.out.println("Enter Field #"+(b+2)+" (max-10 character)");
					answer = console.next();
					if(answer.length()>10){
						answer = answer.substring(0, 10);
					}
					fileWriter.writeBytes(answer);
					fileWriter.readLine();
				}
				for(int b=9; b>recordNum; b--){
					fileWriter.writeBytes("EmptyField");
					fileWriter.readLine();
				}
				System.out.println("Record is created");
			}
			else{
				System.out.println("Not enough space for type file!!");
			}
		}
			
	}
	

	/**
	 * deletes a record
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void deleteRecord(RandomAccessFile fileWriter, Scanner console, File dbms)throws IOException  {
		// TODO Auto-generated method stub
		boolean isEnd = false;
		System.out.println("Enter the which type you want to delete a record");
		boolean isExist = false;
		String current = "", prev = "";
		String typeName = console.next();
		int typeLength = typeName.length();
		Random rand = new Random();
		if(typeName.length() > 10){
			typeName = typeName.substring(0, 10);
		}
		String tempTypeName = typeName;
		for(int t=10; t>typeLength; t--){
			tempTypeName+=" ";
		}
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i=0; i<15; i++){
			prev = current;
			current = fileWriter.readLine();
			if(current.substring(0, 10).equals(tempTypeName)){
				isExist = true;
				break;
			}
		}
		
		if(!isExist){
			System.out.println("Type doesn't exist");
		}
		else{
			int recordNum = 0;
			long position = 0;
			System.out.println("Enter the primary Key of Record you want to delete!");
			String recName = console.next();
			if(recName.length()>10){
				recName = recName.substring(0, 10);
			}
			String tempRecName = recName;
			for(int t=10; t>recName.length(); t--){
				tempRecName+=" ";
			}		
			current = fileWriter.readLine();
			while(!current.substring(0, 10).equals(tempTypeName)){
				current = fileWriter.readLine();
			}
			
			while(!current.substring(0, 10).equals(tempRecName)){
				position = fileWriter.getFilePointer() - 11;
				prev = current;
				current = fileWriter.readLine();
				if(current.substring(0, 7).equals("endType")){
					isEnd = true;
					break;
				}
			}
			if(isEnd || !prev.substring(0, 9).equals("newRecord")){
				System.out.println("There are not any record name exist under this type!!");
			}
			else{
				fileWriter.close();
				fileWriter = new RandomAccessFile(dbms, "rw");
				fileWriter.seek(position);
				for(int l=0; l<12; l++){
					fileWriter.writeBytes("          ");
					fileWriter.readLine();
				}
				System.out.println("Record is deleted!");
			}
			
		}
		
	}
	

	/**
	 * search a record
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void searchRecord(RandomAccessFile fileWriter, Scanner console, File dbms)throws IOException  {
		// TODO Auto-generated method stub
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		boolean isFound = false;
		
		System.out.println("Enter primary Key of a record");
		String pkey = console.next();
		String current = "          ", prev = "          ";
		String tempPKey = pkey;
		for(int t=10; t> pkey.length(); t--){
			tempPKey+=" ";
		}
		
		while(current != null){
			prev = current;
			current = fileWriter.readLine();
			if(current!=null){
				if(prev.substring(0, 9).equals("newRecord") && current.substring(0, 10).equals(tempPKey)){
					for(int jk=0; jk<10; jk++){
						System.out.println(fileWriter.readLine());
					}
					System.out.println();
					isFound = true;
				}
			}		
		}
		
		if(!isFound){
			System.out.println("Record is not found in the Database!!");
		}
	}

	/**
	 * list all records
	 * @param fileWriter for RandomAccessFile
	 * @param console for scanner
	 * @param dbms for file
	 * @throws IOException
	 */
	private static void listRecords(RandomAccessFile fileWriter, Scanner console, File dbms)throws IOException  {
		// TODO Auto-generated method stub
		System.out.println("Enter the type you want to list all records");
		boolean isExist = false;
		String current = "", prev = "";
		String typeName = console.next();
		int typeLength = typeName.length();
		Random rand = new Random();
		if(typeName.length() > 10){
			typeName = typeName.substring(0, 10);
		}
		String tempTypeName = typeName;
		for(int t=10; t>typeLength; t--){
			tempTypeName+=" ";
		}
		
		fileWriter.close();
		fileWriter = new RandomAccessFile(dbms, "rw");
		for(int i=0; i<15; i++){
			prev = current;
			current = fileWriter.readLine();
			if(current.substring(0, 10).equals(tempTypeName)){
				isExist = true;
				break;
			}
		}
		
		if(!isExist){
			System.out.println("Type doesn't exist");
		}
		else{
			current = fileWriter.readLine();
			while(!current.substring(0, 10).equals(tempTypeName)){
				current = fileWriter.readLine();
			}
			while(!current.substring(0, 7).equals("endType")){
				if(current.substring(0, 9).equals("newRecord")){
					fileWriter.readLine();
					for(int jk=0; jk<10; jk++){
						System.out.println(fileWriter.readLine());
					}
					System.out.println();
				}
				current = fileWriter.readLine();
			}		
			
		
		}
	}


}
