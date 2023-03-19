/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.ps;
import java.io.File;
import java.util.Scanner;

/**
 *
 * @author usuario
 */
public class Translator extends PS{
    String linha;
    String upcode;
    String[] parts;
    String instructionName;
    String instructionCode;
    
    public Translator(){ //setter  
    }
    
    public String getUpcode(String line){  //
        this.linha = line; 
        upcode = linha.length() < 2 ? linha : linha.substring(0, 2);
              
        try 
        {
            File table = new File("SymbolTable.txt");
            Scanner input;
            
            input = new Scanner(table);
            
            while ( input.hasNextLine() ) 
            {
                String tableLine = input.nextLine();
                parts = tableLine.split(" ");
                instructionCode = parts[1];
                
                if (upcode.equals(instructionCode)){
                    instructionName = parts[0];
                }
            }
            input.close();

        } catch ( Exception ex ) {
            ex.printStackTrace();
        }
    return instructionName;  
    }
    
}
