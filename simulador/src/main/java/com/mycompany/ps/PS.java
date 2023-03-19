/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Project/Maven2/JavaApp/src/main/java/${packagePath}/${mainClassName}.java to edit this template
 */

package com.mycompany.ps;
import java.io.File;
import java.io.FileReader;
//import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.util.Scanner;
/**
 *
 * @author anna
 */
public class PS {
    static String line;
    static String instructionName;
    static String upcode;
    static String instructionCode;
    
    
    
    public static void main( String[] args ) 
    {
        LeObj(); //call functions in order
    }
    
    public static void LeObj( ) 
    {
        try 
        {
            System.out.print("Insert the name of the file in the following format 'name.txt': ");
            Scanner input = new Scanner( System.in );
            File file = new File( input.nextLine() );

            input = new Scanner( file );
            
            while ( input.hasNextLine() ) 
            {
                line = input.nextLine();
                upcode = line.length() < 2 ? line : line.substring(0, 2);
                Upcode();
            }
            input.close();

        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    }
    /*
    ===================
    Main 
    -------------------
    Receives the name of the file, returns the file printed line by line.
    ===================
    */
    
    
    public static void Upcode(){
        BufferedReader reader;
        try 
        {
            reader = new BufferedReader(new FileReader("SymbolTable.txt"));
            String Tline = reader.readLine();


            while (Tline != null) 
            {
                
                String[] parts = Tline.split("\"\\\\s+\"",2);
                instructionName = parts[0];
                instructionCode = parts[1];
                
                System.out.println(instructionName);
                System.out.println(instructionCode);
                if (upcode.equals(instructionCode)){
                    System.out.println( instructionName );
                }
                else{
                    //System.out.println( "AAAAAA" );
                }
                
                Tline = reader.readLine();
            }

        } catch ( Exception ex ) {
            ex.printStackTrace();
        } 
    }
    /*
    ===================
    Upcode 
    -------------------
    AAAAAAA
    ===================
    */


    
}
