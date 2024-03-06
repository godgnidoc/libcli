import { ArgPass } from "./argument";
/**
 * 短选项
 */
export declare function ShortOpt(opt: string): (target: any, propertyKey: string) => void;
/**
 * 长选项
 */
export declare function LongOpt(opt: string): (target: any, propertyKey: string) => void;
/**
 * 选项描述
 */
export declare function Brief(brief: string): (target: any, propertyKey: string) => void;
/**
 * 选项参数列表或参数检查函数
 */
export declare function Args(args: string[] | ArgPass): (target: any, propertyKey: string) => void;
/**
 * 选项参数补全函数
 */
export declare function Complete(complete: ((arg?: string) => string[] | Promise<string[]>)): (target: any, propertyKey: string) => void;
/**
 * 必要选项
 */
export declare function Required(): (target: any, propertyKey: string) => void;
/**
 * 可重复选项
 */
export declare function Repeat(): (target: any, propertyKey: string) => void;
